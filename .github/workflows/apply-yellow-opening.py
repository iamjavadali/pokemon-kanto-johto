import json
from pathlib import Path


def splice(path, start, end, replacement):
    p = Path(path)
    text = p.read_text()
    if text.count(start) != 1 or text.count(end) != 1:
        raise SystemExit(f"Guard failed for {path}: {start!r} / {end!r}")
    a = text.index(start)
    b = text.index(end, a)
    p.write_text(text[:a] + replacement + text[b:])


# Dedicated unused FRLG hide flag for the temporary wild Pikachu.
p = Path("include/constants/flags_frlg.h")
text = p.read_text()
old = "#define FLAG_0x0AF               0x0AF"
new = "#define FLAG_HIDE_PALLET_WILD_PIKACHU                    0x0AF"
if text.count(old) != 1:
    raise SystemExit("Expected unused FLAG_0x0AF exactly once.")
p.write_text(text.replace(old, new, 1))

# Add a hidden Pikachu object to Pallet Town.
p = Path("data/maps/PalletTown_Frlg/map.json")
data = json.loads(p.read_text())
if any(o.get("local_id") == "LOCALID_YELLOW_PIKACHU" for o in data["object_events"]):
    raise SystemExit("Yellow Pikachu object already exists.")
data["object_events"].append({
    "local_id": "LOCALID_YELLOW_PIKACHU",
    "type": "object",
    "graphics_id": "OBJ_EVENT_GFX_PIKACHU",
    "x": 14,
    "y": 1,
    "elevation": 3,
    "movement_type": "MOVEMENT_TYPE_FACE_LEFT",
    "movement_range_x": 0,
    "movement_range_y": 0,
    "trainer_type": "TRAINER_TYPE_NONE",
    "trainer_sight_or_berry_tree_id": "0",
    "script": "0x0",
    "flag": "FLAG_HIDE_PALLET_WILD_PIKACHU",
})
p.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n")

# Hide the temporary Pikachu during normal Pallet map initialization.
p = Path("data/maps/PalletTown_Frlg/scripts.inc")
text = p.read_text()
anchor = "PalletTown_OnTransition::\n\tsetworldmapflag FLAG_WORLD_MAP_PALLET_TOWN\n"
replacement = anchor + "\tcall_if_eq VAR_MAP_SCENE_PALLET_TOWN_OAK, 0, PalletTown_EventScript_HideYellowPikachu\n"
if text.count(anchor) != 1:
    raise SystemExit("Pallet transition anchor mismatch.")
text = text.replace(anchor, replacement, 1)
helper_anchor = "PalletTown_EventScript_TryReadySignLady::\n"
helper = "PalletTown_EventScript_HideYellowPikachu::\n\tsetflag FLAG_HIDE_PALLET_WILD_PIKACHU\n\treturn\n\n"
if text.count(helper_anchor) != 1:
    raise SystemExit("Pallet helper anchor mismatch.")
text = text.replace(helper_anchor, helper + helper_anchor, 1)
p.write_text(text)

# Canonical Yellow event order in Pallet Town.
pallet_opening = r'''PalletTown_EventScript_OakTrigger::
	famechecker FAMECHECKER_OAK, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	textcolor NPC_TEXT_COLOR_MALE
	delay 30
	playbgm MUS_RG_OAK, 0
	message PalletTown_Text_OakDontGoOut
	waitmessage
	delay 85
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	delay 30
	addobject LOCALID_PALLET_PROF_OAK
	call_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_OakEnterLeft
	call_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_OakEnterRight
	delay 30
	msgbox PalletTown_Text_OakGrassUnsafeNeedMon
	closemessage
	call_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_ShowWildPikachuLeft
	call_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_ShowWildPikachuRight
	playmoncry SPECIES_PIKACHU, CRY_MODE_ENCOUNTER
	waitmoncry
	msgbox PalletTown_Text_WildPikachuAppeared
	closemessage
	delay 20
	msgbox PalletTown_Text_OakCatchPikachu
	closemessage
	playse SE_BALL_THROW
	delay 40
	removeobject LOCALID_YELLOW_PIKACHU
	setflag FLAG_HIDE_PALLET_WILD_PIKACHU
	delay 30
	msgbox PalletTown_Text_OakCaughtPikachu
	msgbox PalletTown_Text_OakComeWithMeYellow
	closemessage
	delay 20
	call_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_OakLeadPlayerToLabLeft
	call_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_OakLeadPlayerToLabRight
	opendoor 16, 13
	waitdooranim
	applymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakEnterLab
	applymovement LOCALID_PLAYER, PalletTown_Movement_PlayerEnterLab
	waitmovement 0
	closedoor 16, 13
	waitdooranim
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 1
	clearflag FLAG_HIDE_OAK_IN_HIS_LAB
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 1
	setflag FLAG_HIDE_OAK_IN_PALLET_TOWN
	setflag FLAG_HIDE_BULBASAUR_BALL
	clearflag FLAG_HIDE_SQUIRTLE_BALL
	setflag FLAG_HIDE_CHARMANDER_BALL
	clearflag FLAG_HIDE_RIVAL_IN_LAB
	setflag FLAG_DONT_TRANSITION_MUSIC
	warp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 12
	waitstate
	releaseall
	end

PalletTown_EventScript_ShowWildPikachuLeft::
	setobjectxyperm LOCALID_YELLOW_PIKACHU, 13, 1
	setobjectmovementtype LOCALID_YELLOW_PIKACHU, MOVEMENT_TYPE_FACE_LEFT
	clearflag FLAG_HIDE_PALLET_WILD_PIKACHU
	addobject LOCALID_YELLOW_PIKACHU
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	applymovement LOCALID_PALLET_PROF_OAK, Common_Movement_FaceRight
	waitmovement 0
	return

PalletTown_EventScript_ShowWildPikachuRight::
	setobjectxyperm LOCALID_YELLOW_PIKACHU, 12, 1
	setobjectmovementtype LOCALID_YELLOW_PIKACHU, MOVEMENT_TYPE_FACE_RIGHT
	clearflag FLAG_HIDE_PALLET_WILD_PIKACHU
	addobject LOCALID_YELLOW_PIKACHU
	applymovement LOCALID_PLAYER, Common_Movement_FaceLeft
	applymovement LOCALID_PALLET_PROF_OAK, Common_Movement_FaceLeft
	waitmovement 0
	return

'''
splice(
    "data/maps/PalletTown_Frlg/scripts.inc",
    "PalletTown_EventScript_OakTrigger::\n",
    "PalletTown_EventScript_OakEnterLeft::\n",
    pallet_opening,
)

# Pallet dialogue for the Yellow catch sequence.
p = Path("data/maps/PalletTown_Frlg/scripts.inc")
text = p.read_text()
text_anchor = "PalletTown_Text_RaisingMonsToo::\n"
yellow_text = r'''PalletTown_Text_WildPikachuAppeared::
	.string "A wild PIKACHU appeared in the\n"
	.string "tall grass!$"

PalletTown_Text_OakCatchPikachu::
	.string "OAK: Whew... Hold on!\p"
	.string "I'll take care of this one.$"

PalletTown_Text_OakCaughtPikachu::
	.string "OAK: Got it!\p"
	.string "That PIKACHU is safely caught.$"

PalletTown_Text_OakComeWithMeYellow::
	.string "OAK: A POKéMON can appear at any\n"
	.string "time in tall grass.\p"
	.string "You need a POKéMON of your own\n"
	.string "for protection.\p"
	.string "Come with me to my LAB.$"

'''
if text.count(text_anchor) != 1:
    raise SystemExit("Pallet text anchor mismatch.")
text = text.replace(text_anchor, yellow_text + text_anchor, 1)
p.write_text(text)

# Rival is already waiting in the lab; Oak offers the single Eevee ball.
lab_intro = r'''PalletTown_ProfessorOaksLab_ChooseStarterScene::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_OAKS_LAB_PROF_OAK, PalletTown_ProfessorOaksLab_Movement_OakEnter
	waitmovement 0
	removeobject LOCALID_OAKS_LAB_PROF_OAK
	setobjectxyperm LOCALID_OAKS_LAB_PROF_OAK, 6, 3
	setobjectmovementtype LOCALID_OAKS_LAB_PROF_OAK, MOVEMENT_TYPE_FACE_DOWN
	clearflag FLAG_HIDE_OAK_IN_HIS_LAB
	applymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerEnter
	waitmovement 0
	clearflag FLAG_DONT_TRANSITION_MUSIC
	fadedefaultbgm
	applymovement LOCALID_OAKS_LAB_RIVAL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox PalletTown_ProfessorOaksLab_Text_RivalFedUpWithWaiting
	msgbox PalletTown_ProfessorOaksLab_Text_OakRivalCameEarly
	msgbox PalletTown_ProfessorOaksLab_Text_OakTakeBallOnTable
	msgbox PalletTown_ProfessorOaksLab_Text_RivalNoFairWhatAboutMe
	msgbox PalletTown_ProfessorOaksLab_Text_OakBePatientYellow
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 2
	releaseall
	end

'''
splice(
    "data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc",
    "PalletTown_ProfessorOaksLab_ChooseStarterScene::\n",
    "PalletTown_ProfessorOaksLab_Movement_OakEnter::\n",
    lab_intro,
)

# Center ball becomes Yellow's Eevee ball. Rival physically takes it before Pikachu is gifted.
eevee_ball = r'''PalletTown_ProfessorOaksLab_EventScript_SquirtleBall::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	goto_if_ne VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 2, PalletTown_ProfessorOaksLab_EventScript_LastPokeBall
	applymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_RivalRushToEevee
	waitmovement 0
	msgbox PalletTown_ProfessorOaksLab_Text_RivalWantsEevee
	closemessage
	applymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerPushedFromEevee
	applymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_RivalSnatchEevee
	waitmovement 0
	removeobject LOCALID_SQUIRTLE_BALL
	setflag FLAG_HIDE_SQUIRTLE_BALL
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	msgbox PalletTown_ProfessorOaksLab_Text_RivalSnatchedEevee
	waitfanfare
	msgbox PalletTown_ProfessorOaksLab_Text_OakRivalWhatDoing
	msgbox PalletTown_ProfessorOaksLab_Text_RivalKeepingEevee
	msgbox PalletTown_ProfessorOaksLab_Text_OakAllowsEevee
	closemessage
	applymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerWalkToOakYellow
	waitmovement 0
	msgbox PalletTown_ProfessorOaksLab_Text_OakGiveCaughtPikachu
	setvar PLAYER_STARTER_NUM, 2
	setvar PLAYER_STARTER_SPECIES, SPECIES_PIKACHU
	setvar RIVAL_STARTER_SPECIES, SPECIES_EEVEE
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_PALLET_LADY_NOT_BLOCKING_SIGN
	givemon PLAYER_STARTER_SPECIES, 5
	setmonmove 0, 0, MOVE_THUNDER_SHOCK
	setmonmove 0, 1, MOVE_GROWL
	setmonmove 0, 2, MOVE_NONE
	setmonmove 0, 3, MOVE_NONE
	copyvar VAR_STARTER_MON, PLAYER_STARTER_NUM
	bufferspeciesname STR_VAR_1, PLAYER_STARTER_SPECIES
	message PalletTown_ProfessorOaksLab_Text_ReceivedMonFromOak
	waitmessage
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	waitfanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PalletTown_ProfessorOaksLab_EventScript_GiveYellowPikachuNickname
	goto PalletTown_ProfessorOaksLab_EventScript_FinishYellowStarterGift
	end

PalletTown_ProfessorOaksLab_EventScript_GiveYellowPikachuNickname::
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
	goto PalletTown_ProfessorOaksLab_EventScript_FinishYellowStarterGift
	end

PalletTown_ProfessorOaksLab_EventScript_FinishYellowStarterGift::
	closemessage
	applymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_RivalReadyAfterEevee
	waitmovement 0
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 3
	call_if_set FLAG_OPENED_START_MENU, PalletTown_ProfessorOaksLab_EventScript_ReadyEndSignLadyScene
	releaseall
	end

PalletTown_ProfessorOaksLab_Movement_RivalRushToEevee::
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

PalletTown_ProfessorOaksLab_Movement_PlayerPushedFromEevee::
	walk_right
	step_end

PalletTown_ProfessorOaksLab_Movement_RivalSnatchEevee::
	walk_right
	walk_in_place_faster_up
	step_end

PalletTown_ProfessorOaksLab_Movement_PlayerWalkToOakYellow::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_in_place_faster_up
	step_end

PalletTown_ProfessorOaksLab_Movement_RivalReadyAfterEevee::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

'''
splice(
    "data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc",
    "PalletTown_ProfessorOaksLab_EventScript_SquirtleBall::\n",
    "PalletTown_ProfessorOaksLab_EventScript_CharmanderBall::\n",
    eevee_ball,
)

# Rival begins the exit battle from x5,y5 after taking Eevee.
p = Path("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
text = p.read_text()
move_replacements = [
    (
        "PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachLeft::\n\twalk_down\n\twalk_down\n\twalk_down\n\tstep_end\n",
        "PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachLeft::\n\twalk_down\n\twalk_down\n\tstep_end\n",
    ),
    (
        "PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachMid::\n\twalk_right\n\twalk_down\n\twalk_down\n\twalk_down\n\tstep_end\n",
        "PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachMid::\n\twalk_right\n\twalk_down\n\twalk_down\n\tstep_end\n",
    ),
    (
        "PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachRight::\n\twalk_right\n\twalk_right\n\twalk_down\n\twalk_down\n\twalk_down\n\tstep_end\n",
        "PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachRight::\n\twalk_right\n\twalk_right\n\twalk_down\n\twalk_down\n\tstep_end\n",
    ),
]
for old_move, new_move in move_replacements:
    if text.count(old_move) != 1:
        raise SystemExit("Yellow rival approach movement guard failed.")
    text = text.replace(old_move, new_move, 1)
p.write_text(text)

# Pikachu exits its ball after the rival leaves; Oak explains the follower behavior.
p = Path("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
text = p.read_text()
old_finish = r'''PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit::
	removeobject LOCALID_OAKS_LAB_RIVAL
	playse SE_EXIT
	fadedefaultbgm
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 4
	setflag FLAG_BEAT_RIVAL_IN_OAKS_LAB
	callnative UpdateFollowingPokemon
	releaseall
	end
'''
new_finish = r'''PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit::
	removeobject LOCALID_OAKS_LAB_RIVAL
	playse SE_EXIT
	fadedefaultbgm
	setflag FLAG_BEAT_RIVAL_IN_OAKS_LAB
	callnative UpdateFollowingPokemon
	delay 30
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	waitmoncry
	msgbox PalletTown_ProfessorOaksLab_Text_OakPikachuCameOut
	msgbox PalletTown_ProfessorOaksLab_Text_OakPikachuDislikesBalls
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 4
	releaseall
	end
'''
if text.count(old_finish) != 1:
    raise SystemExit("Post-rival Yellow finish guard failed.")
text = text.replace(old_finish, new_finish, 1)

text_anchor = "PalletTown_ProfessorOaksLab_Text_OakReadyForStarter::\n"
yellow_lab_text = r'''PalletTown_ProfessorOaksLab_Text_OakRivalCameEarly::
	.string "OAK: {RIVAL}? You're early.\p"
	.string "I told you to come by later.\p"
	.string "Well, just wait there for now.$"

PalletTown_ProfessorOaksLab_Text_OakTakeBallOnTable::
	.string "OAK: {PLAYER}, see that POKé BALL\n"
	.string "on the table?\p"
	.string "There's a POKéMON inside.\n"
	.string "You may have it. Go on, take it!$"

PalletTown_ProfessorOaksLab_Text_OakBePatientYellow::
	.string "OAK: Be patient, {RIVAL}.\p"
	.string "I planned to give you a POKéMON,\n"
	.string "too. Just wait.$"

PalletTown_ProfessorOaksLab_Text_RivalWantsEevee::
	.string "{RIVAL}: No way!\p"
	.string "{PLAYER}, I want this POKéMON!$"

PalletTown_ProfessorOaksLab_Text_RivalSnatchedEevee::
	.string "{RIVAL} pushed ahead and snatched\n"
	.string "the POKé BALL!\p"
	.string "The POKéMON inside is EEVEE!$"

PalletTown_ProfessorOaksLab_Text_OakRivalWhatDoing::
	.string "OAK: {RIVAL}! What are you doing?$"

PalletTown_ProfessorOaksLab_Text_RivalKeepingEevee::
	.string "{RIVAL}: Gramps, I want this one!$"

PalletTown_ProfessorOaksLab_Text_OakAllowsEevee::
	.string "OAK: But I... Oh, all right.\p"
	.string "EEVEE is yours. I was going to\n"
	.string "give you a POKéMON anyway.\p"
	.string "{PLAYER}, come over here.$"

PalletTown_ProfessorOaksLab_Text_OakGiveCaughtPikachu::
	.string "OAK: {PLAYER}, this is the POKéMON\n"
	.string "I caught outside earlier.\p"
	.string "You can have it. It's still wild\n"
	.string "at heart and isn't tame yet.$"

PalletTown_ProfessorOaksLab_Text_OakPikachuCameOut::
	.string "OAK: What? PIKACHU came out of its\n"
	.string "POKé BALL on its own!$"

PalletTown_ProfessorOaksLab_Text_OakPikachuDislikesBalls::
	.string "OAK: Would you look at that!\p"
	.string "It seems your PIKACHU doesn't like\n"
	.string "staying inside POKé BALLS.\p"
	.string "Keep it with you instead. You can\n"
	.string "talk to it and see how it feels.$"

'''
if text.count(text_anchor) != 1:
    raise SystemExit("Yellow lab text anchor mismatch.")
text = text.replace(text_anchor, yellow_lab_text + text_anchor, 1)
p.write_text(text)

# Lock the exact sequence in project documentation.
p = Path("docs/projects/KANTO_JOHTO_MASTER_RULES.md")
text = p.read_text().rstrip() + "\n"
heading = "## Canonical Pokémon Yellow opening contract"
if heading not in text:
    text += """
## Canonical Pokémon Yellow opening contract

The Kanto opening must preserve the Pokémon Yellow starter sequence in order: the player attempts to leave Pallet Town; Oak stops and approaches the player; a wild level-5 Pikachu appears in the grass and Oak catches it; Oak leads the player to his lab; the rival is already waiting; Oak offers the player the single Poké Ball containing Eevee; the rival pushes ahead and takes Eevee; Oak gives the player the same Pikachu caught outside; the rival challenges the player with Eevee; after the battle the rival leaves; Pikachu refuses to remain in its Poké Ball; Oak explains the behavior; and Pikachu begins following the player. No implementation pass may omit, reorder, or replace these story beats.
"""
p.write_text(text)

# Structural guards before build.
checks = {
    "data/maps/PalletTown_Frlg/scripts.inc": [
        "LOCALID_YELLOW_PIKACHU",
        "PalletTown_Text_OakCaughtPikachu",
        "clearflag FLAG_HIDE_RIVAL_IN_LAB",
    ],
    "data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc": [
        "PalletTown_ProfessorOaksLab_Text_RivalWantsEevee",
        "SPECIES_PIKACHU",
        "SPECIES_EEVEE",
        "PalletTown_ProfessorOaksLab_Text_OakPikachuDislikesBalls",
    ],
    "include/constants/flags_frlg.h": ["FLAG_HIDE_PALLET_WILD_PIKACHU"],
}
for filename, needles in checks.items():
    body = Path(filename).read_text()
    for needle in needles:
        if needle not in body:
            raise SystemExit(f"Missing required contract token {needle} in {filename}")
