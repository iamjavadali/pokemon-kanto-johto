from pathlib import Path
import re

def replace_once(text, old, new, label):
    count = text.count(old)
    if count != 1:
        raise SystemExit(f"{label}: expected exactly 1 match, found {count}")
    return text.replace(old, new, 1)

oak_speech_path = Path("src/oak_speech.c")
pallet_path = Path("data/maps/PalletTown_Frlg/scripts.inc")
lab_path = Path("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
trainers_path = Path("src/data/trainers_frlg.party")
docs_path = Path("docs/projects/KANTO_JOHTO_MASTER_RULES.md")

oak_speech = oak_speech_path.read_text()
pallet = pallet_path.read_text()
lab = lab_path.read_text()
trainers = trainers_path.read_text()
docs = docs_path.read_text()

# 1) Protagonist naming: show the same preset/custom choice flow used for the rival.
old_player_name = """            tTrainerPicPosX = 0;
            OakSpeechPrintMessage(gOakSpeech_Text_YourNameWhatIsIt, sOakSpeechResources->textSpeed, FALSE);
            gTasks[taskId].func = Task_OakSpeech_FadeOutForPlayerNamingScreen;
"""
new_player_name = """            tTrainerPicPosX = 0;
            sOakSpeechResources->hasPlayerBeenNamed = FALSE;
            OakSpeechPrintMessage(gOakSpeech_Text_YourNameWhatIsIt, sOakSpeechResources->textSpeed, FALSE);
            gTasks[taskId].func = Task_OakSpeech_MoveRivalDisplayNameOptions;
"""
oak_speech = replace_once(oak_speech, old_player_name, new_player_name, "player preset/custom naming entry")

old_player_reject = """        if (sOakSpeechResources->hasPlayerBeenNamed == FALSE)
            gTasks[taskId].func = Task_OakSpeech_FadeOutForPlayerNamingScreen;
        else
            gTasks[taskId].func = Task_OakSpeech_RepeatNameQuestion;
"""
new_player_reject = """        gTasks[taskId].func = Task_OakSpeech_RepeatNameQuestion;
"""
oak_speech = replace_once(oak_speech, old_player_reject, new_player_reject, "name confirmation retry")

# 2) Pallet staging: keep rival hidden until the lab conversation and make Oak explicit.
old_warp = """\tsetflag FLAG_HIDE_OAK_IN_PALLET_TOWN
\tsetflag FLAG_DONT_TRANSITION_MUSIC
\twarp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 12
"""
new_warp = """\tsetflag FLAG_HIDE_OAK_IN_PALLET_TOWN
\tsetflag FLAG_HIDE_RIVAL_IN_LAB
\tsetflag FLAG_DONT_TRANSITION_MUSIC
\twarp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 12
"""
pallet = replace_once(pallet, old_warp, new_warp, "hide rival before lab entry")

old_oak_grass_text = """PalletTown_Text_OakGrassUnsafeNeedMon::
\t.string "OAK: It's unsafe!\\n"
\t.string "Wild POKéMON live in tall grass!\\p"
\t.string "You need your own POKéMON for\\n"
\t.string "your protection.\\p"
\t.string "I know!\\n"
\t.string "Here, come with me!$"
"""
new_oak_grass_text = """PalletTown_Text_OakGrassUnsafeNeedMon::
\t.string "OAK: It's unsafe!\\n"
\t.string "Wild POKéMON live in tall grass!\\p"
\t.string "You need your own POKéMON for\\n"
\t.string "your protection.\\p"
\t.string "I have one for you at my LAB.\\n"
\t.string "Come with me!$"
"""
pallet = replace_once(pallet, old_oak_grass_text, new_oak_grass_text, "Oak grass dialogue")

# 3) Yellow-inspired Oak Lab scene.
scene_start = lab.index("PalletTown_ProfessorOaksLab_ChooseStarterScene::")
scene_end = lab.index("PalletTown_ProfessorOaksLab_Movement_OakEnter::", scene_start)
old_scene = lab[scene_start:scene_end]
if "SPECIES_SQUIRTLE" not in old_scene or "callnative UpdateFollowingPokemon" not in old_scene:
    raise SystemExit("starter scene guard failed: expected Step 1 v0.1 block not found")

new_scene = """PalletTown_ProfessorOaksLab_ChooseStarterScene::
\tlockall
\ttextcolor NPC_TEXT_COLOR_MALE
\tapplymovement LOCALID_OAKS_LAB_PROF_OAK, PalletTown_ProfessorOaksLab_Movement_OakEnter
\twaitmovement 0
\tremoveobject LOCALID_OAKS_LAB_PROF_OAK
\tsetobjectxyperm LOCALID_OAKS_LAB_PROF_OAK, 6, 3
\tsetobjectmovementtype LOCALID_OAKS_LAB_PROF_OAK, MOVEMENT_TYPE_FACE_DOWN
\tclearflag FLAG_HIDE_OAK_IN_HIS_LAB
\tapplymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerEnter
\twaitmovement 0
\tclearflag FLAG_DONT_TRANSITION_MUSIC
\tsavebgm MUS_DUMMY
\tfadedefaultbgm
\tmsgbox PalletTown_ProfessorOaksLab_Text_OakReadyForStarter
\tclosemessage
\tdelay 30
\tsetobjectxyperm LOCALID_OAKS_LAB_RIVAL, 5, 10
\tsetobjectmovementtype LOCALID_OAKS_LAB_RIVAL, MOVEMENT_TYPE_FACE_UP
\tclearflag FLAG_HIDE_RIVAL_IN_LAB
\taddobject LOCALID_OAKS_LAB_RIVAL
\tplaybgm MUS_RG_ENCOUNTER_RIVAL, 0
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_RivalEnter
\twaitmovement 0
\tmsgbox PalletTown_ProfessorOaksLab_Text_RivalFedUpWithWaiting
\tmsgbox PalletTown_ProfessorOaksLab_Text_RivalAlreadyHasEevee
\tclosemessage
\tdelay 30
\tremoveobject LOCALID_BULBASAUR_BALL
\tremoveobject LOCALID_SQUIRTLE_BALL
\tremoveobject LOCALID_CHARMANDER_BALL
\tsetflag FLAG_HIDE_BULBASAUR_BALL
\tsetflag FLAG_HIDE_SQUIRTLE_BALL
\tsetflag FLAG_HIDE_CHARMANDER_BALL
\tmsgbox PalletTown_ProfessorOaksLab_Text_OakStartersAlreadyClaimed
\tclosemessage
\tdelay 30
\tmsgbox PalletTown_ProfessorOaksLab_Text_OakSavedOnePartner
\tsetvar PLAYER_STARTER_NUM, 2
\tsetvar PLAYER_STARTER_SPECIES, SPECIES_PIKACHU
\tsetvar RIVAL_STARTER_SPECIES, SPECIES_EEVEE
\tsetflag FLAG_SYS_POKEMON_GET
\tsetflag FLAG_PALLET_LADY_NOT_BLOCKING_SIGN
\tgivemon PLAYER_STARTER_SPECIES, 5
\tsetmonmove 0, 0, MOVE_THUNDER_SHOCK
\tsetmonmove 0, 1, MOVE_GROWL
\tsetmonmove 0, 2, MOVE_NONE
\tsetmonmove 0, 3, MOVE_NONE
\tcopyvar VAR_STARTER_MON, PLAYER_STARTER_NUM
\tbufferspeciesname STR_VAR_1, PLAYER_STARTER_SPECIES
\tmessage PalletTown_ProfessorOaksLab_Text_ReceivedMonFromOak
\twaitmessage
\tplayfanfare MUS_RG_OBTAIN_KEY_ITEM
\twaitfanfare
\tmsgbox gText_NicknameThisPokemon, MSGBOX_YESNO
\tgoto_if_eq VAR_RESULT, YES, PalletTown_ProfessorOaksLab_EventScript_GivePikachuNickname
\tgoto PalletTown_ProfessorOaksLab_EventScript_FinishPikachuStarter
\tend

PalletTown_ProfessorOaksLab_EventScript_GivePikachuNickname::
\tsetvar VAR_0x8004, 0
\tcall Common_EventScript_NameReceivedPartyMon
\tgoto PalletTown_ProfessorOaksLab_EventScript_FinishPikachuStarter
\tend

PalletTown_ProfessorOaksLab_EventScript_FinishPikachuStarter::
\tclosemessage
\tmsgbox PalletTown_ProfessorOaksLab_Text_RivalAlreadyChose
\tsetvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 3
\tcall_if_set FLAG_OPENED_START_MENU, PalletTown_ProfessorOaksLab_EventScript_ReadyEndSignLadyScene
\treleaseall
\tend

"""
lab = lab[:scene_start] + new_scene + lab[scene_end:]

# 4) Dedicated first-battle approach: rival comes to the player's column and stops adjacent.
battle_start = lab.index("PalletTown_ProfessorOaksLab_EventScript_RivalBattle::")
battle_end = lab.index("@ The scripts for the rival approaching the player for battle are a bit bloated", battle_start)
old_battle = lab[battle_start:battle_end]
if "VAR_STARTER_MON" not in old_battle:
    raise SystemExit("rival battle guard failed: legacy starter routing not found")

new_battle = """PalletTown_ProfessorOaksLab_EventScript_RivalBattle::
\ttextcolor NPC_TEXT_COLOR_MALE
\tplaybgm MUS_RG_ENCOUNTER_RIVAL, 0
\tapplymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
\twaitmovement 0
\tgoto_if_eq VAR_TEMP_2, 1, PalletTown_ProfessorOaksLab_EventScript_YellowRivalApproachLeft
\tgoto_if_eq VAR_TEMP_2, 2, PalletTown_ProfessorOaksLab_EventScript_YellowRivalApproachMid
\tgoto_if_eq VAR_TEMP_2, 3, PalletTown_ProfessorOaksLab_EventScript_YellowRivalApproachRight
\tend

PalletTown_ProfessorOaksLab_EventScript_YellowRivalApproachLeft::
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachLeft
\twaitmovement 0
\tgoto PalletTown_ProfessorOaksLab_EventScript_YellowRivalStartBattle
\tend

PalletTown_ProfessorOaksLab_EventScript_YellowRivalApproachMid::
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachMid
\twaitmovement 0
\tgoto PalletTown_ProfessorOaksLab_EventScript_YellowRivalStartBattle
\tend

PalletTown_ProfessorOaksLab_EventScript_YellowRivalApproachRight::
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachRight
\twaitmovement 0
\tgoto PalletTown_ProfessorOaksLab_EventScript_YellowRivalStartBattle
\tend

PalletTown_ProfessorOaksLab_EventScript_YellowRivalStartBattle::
\tapplymovement LOCALID_OAKS_LAB_RIVAL, Common_Movement_WalkInPlaceFasterDown
\tapplymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
\twaitmovement 0
\tmsgbox PalletTown_ProfessorOaksLab_Text_RivalLetsCheckOutMons
\tclosemessage
\tapplymovement LOCALID_OAKS_LAB_PROF_OAK, Common_Movement_FaceDown
\twaitmovement 0
\ttrainerbattle_earlyrival TRAINER_RIVAL_OAKS_LAB_SQUIRTLE, RIVAL_BATTLE_TUTORIAL, PalletTown_ProfessorOaksLab_Text_RivalDefeat, Text_RivalVictory
\tgoto PalletTown_ProfessorOaksLab_EventScript_EndRivalBattle
\tend

PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachLeft::
\twalk_down
\twalk_down
\twalk_down
\tstep_end

PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachMid::
\twalk_right
\twalk_down
\twalk_down
\twalk_down
\tstep_end

PalletTown_ProfessorOaksLab_Movement_YellowRivalApproachRight::
\twalk_right
\twalk_right
\twalk_down
\twalk_down
\twalk_down
\tstep_end

"""
lab = lab[:battle_start] + new_battle + lab[battle_end:]

# 5) Dedicated clean exit and activate Pikachu follower after the battle.
old_end = """PalletTown_ProfessorOaksLab_EventScript_EndRivalBattle::
\tspecial HealPlayerParty
\tmsgbox PalletTown_ProfessorOaksLab_Text_RivalGoToughenMyMon
\tclosemessage
\tplaybgm MUS_RG_RIVAL_EXIT, 0
\tcall_if_eq VAR_TEMP_2, 1, PalletTown_ProfessorOaksLab_EventScript_RivalExitAfterBattleLeft
\tcall_if_eq VAR_TEMP_2, 2, PalletTown_ProfessorOaksLab_EventScript_RivalExitAfterBattleMid
\tcall_if_eq VAR_TEMP_2, 3, PalletTown_ProfessorOaksLab_EventScript_RivalExitAfterBattleRight
\tremoveobject LOCALID_OAKS_LAB_RIVAL
\tplayse SE_EXIT
\tfadedefaultbgm
\tsetvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 4
\tsetflag FLAG_BEAT_RIVAL_IN_OAKS_LAB
\treleaseall
\tend
"""
new_end = """PalletTown_ProfessorOaksLab_EventScript_EndRivalBattle::
\tspecial HealPlayerParty
\tmsgbox PalletTown_ProfessorOaksLab_Text_RivalGoToughenMyMon
\tclosemessage
\tplaybgm MUS_RG_RIVAL_EXIT, 0
\tgoto_if_eq VAR_TEMP_2, 1, PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitLeft
\tgoto_if_eq VAR_TEMP_2, 2, PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitMid
\tgoto_if_eq VAR_TEMP_2, 3, PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitRight
\tend

PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitLeft::
\tapplymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerStepRightWatchRival
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalExitStraight
\twaitmovement LOCALID_OAKS_LAB_RIVAL
\tgoto PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit
\tend

PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitMid::
\tapplymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerStepRightWatchRival
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalExitStraight
\twaitmovement LOCALID_OAKS_LAB_RIVAL
\tgoto PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit
\tend

PalletTown_ProfessorOaksLab_EventScript_YellowRivalExitRight::
\tapplymovement LOCALID_PLAYER, PalletTown_ProfessorOaksLab_Movement_PlayerStepLeftWatchRival
\tapplymovement LOCALID_OAKS_LAB_RIVAL, PalletTown_ProfessorOaksLab_Movement_YellowRivalExitStraight
\twaitmovement LOCALID_OAKS_LAB_RIVAL
\tgoto PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit
\tend

PalletTown_ProfessorOaksLab_EventScript_FinishYellowRivalExit::
\tremoveobject LOCALID_OAKS_LAB_RIVAL
\tplayse SE_EXIT
\tfadedefaultbgm
\tsetvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 4
\tsetflag FLAG_BEAT_RIVAL_IN_OAKS_LAB
\tcallnative UpdateFollowingPokemon
\treleaseall
\tend

PalletTown_ProfessorOaksLab_Movement_YellowRivalExitStraight::
\twalk_down
\twalk_down
\twalk_down
\twalk_down
\twalk_down
\tstep_end

PalletTown_ProfessorOaksLab_Movement_PlayerStepRightWatchRival::
\twalk_right
\twalk_in_place_faster_left
\tstep_end

PalletTown_ProfessorOaksLab_Movement_PlayerStepLeftWatchRival::
\twalk_left
\twalk_in_place_faster_right
\tstep_end
"""
lab = replace_once(lab, old_end, new_end, "first rival battle exit")

# 6) Opening dialogue for the adapted Yellow scene.
old_custom_text = """PalletTown_ProfessorOaksLab_Text_OakStartersAlreadyClaimed::
\t.string "OAK: Ah... You're late, {PLAYER}!\\n"
\t.string "The three POKéMON I prepared have\\n"
\t.string "already been claimed by TRAINERS.$"

PalletTown_ProfessorOaksLab_Text_OakSavedOnePartner::
\t.string "But I do have one partner left.\\p"
\t.string "It's unusual, and rather spirited.\\n"
\t.string "I think the two of you may suit\\n"
\t.string "each other.$"

PalletTown_ProfessorOaksLab_Text_RivalAlreadyChose::
\t.string "{RIVAL}: Hah! Took you long enough!\\n"
\t.string "I've already got my POKéMON.\\p"
\t.string "Let's see what that PIKACHU can do!$"
"""
new_custom_text = """PalletTown_ProfessorOaksLab_Text_OakReadyForStarter::
\t.string "OAK: All right, {PLAYER}.\\p"
\t.string "You shouldn't enter tall grass\\n"
\t.string "without a POKéMON of your own.\\p"
\t.string "I brought you here to fix that.$"

PalletTown_ProfessorOaksLab_Text_RivalAlreadyHasEevee::
\t.string "{RIVAL}: I've already got EEVEE!\\n"
\t.string "We've been waiting on you!$"

PalletTown_ProfessorOaksLab_Text_OakStartersAlreadyClaimed::
\t.string "OAK: BULBASAUR, CHARMANDER, and\\n"
\t.string "SQUIRTLE have already been claimed\\l"
\t.string "by other TRAINERS.$"

PalletTown_ProfessorOaksLab_Text_OakSavedOnePartner::
\t.string "But I caught one more POKéMON\\n"
\t.string "today.\\p"
\t.string "It's lively and a little unusual.\\n"
\t.string "I think it should go with you.$"

PalletTown_ProfessorOaksLab_Text_RivalAlreadyChose::
\t.string "{RIVAL}: So you got PIKACHU?\\p"
\t.string "EEVEE and I are ready.\\n"
\t.string "Let's see what it can do!$"
"""
lab = replace_once(lab, old_custom_text, new_custom_text, "custom Yellow opening dialogue")

# 7) First rival party is Yellow's level 5 Eevee. Keep the legacy trainer ID to avoid shifting IDs.
match = re.search(
    r"=== TRAINER_RIVAL_OAKS_LAB_SQUIRTLE ===\n.*?(?=\n=== |\Z)",
    trainers,
    flags=re.S,
)
if not match:
    raise SystemExit("trainer guard failed: Oak Lab Squirtle trainer section missing")
section = match.group(0)
if section.count("\nSquirtle\n") != 1 or "Level: 5" not in section or "- Tackle" not in section or "- Tail Whip" not in section:
    raise SystemExit("trainer guard failed: expected original Lv5 Squirtle/Tackle/Tail Whip section")
new_section = section.replace("\nSquirtle\n", "\nEevee\n", 1)
trainers = trainers[:match.start()] + new_section + trainers[match.end():]

# 8) Project documentation reflects the corrected Step 1 contract.
docs = docs.replace(
    "- FRLG new-game protagonist gender/name selection is retained.\n",
    "- FRLG new-game protagonist gender selection is retained; protagonist naming now offers preset names or a custom typed name, and no protagonist name is auto-selected.\n",
)
docs = docs.replace(
    "- HGSS-style lead-party follower support is enabled and refreshed immediately after Pikachu joins the party.\n",
    "- HGSS-style lead-party follower support is enabled; Pikachu begins following after the first rival battle so the opening battle can stage the two trainers face-to-face.\n",
)
docs = docs.replace(
    "- Prototype note: the existing Squirtle rival branch is reused for the first rival battle to preserve the established FRLG progression while the final Kanto rival team design is still pending.\n",
    "- The opening is Yellow-inspired: Oak intercepts the player at the tall grass, brings them to the lab while the three Kanto starter balls are still visible, the player-named rival enters, the three unavailable starter balls are removed, Oak gives Pikachu, and the rival challenges with a level 5 Eevee.\n- Starter Pikachu is level 5 with Yellow-faithful opening moves Thunder Shock and Growl only; later moves are learned normally.\n- The first rival battle uses dedicated positioning so the rival approaches the player directly instead of reusing the obsolete three-starter movement paths.\n",
)

for path, text in [
    (oak_speech_path, oak_speech),
    (pallet_path, pallet),
    (lab_path, lab),
    (trainers_path, trainers),
    (docs_path, docs),
]:
    path.write_text(text)

print("Step 1 Yellow-inspired correction patch applied successfully.")
