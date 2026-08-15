from pathlib import Path
import re


def replace_once(text, old, new, desc):
    count = text.count(old)
    if count != 1:
        raise SystemExit(f"{desc}: expected 1 exact match, found {count}")
    return text.replace(old, new, 1)


def replace_section(text, start_label, next_label, lines, desc):
    pattern = re.compile(
        rf"(?ms)^{re.escape(start_label)}::\n.*?(?=^{re.escape(next_label)}::)"
    )
    matches = list(pattern.finditer(text))
    if len(matches) != 1:
        raise SystemExit(f"{desc}: expected 1 section, found {len(matches)}")
    replacement = start_label + "::\n" + "\n".join(lines) + "\n\n"
    return pattern.sub(lambda _: replacement, text, count=1)


# Route 1: continue from the actual post-capture coordinates, then use a
# silent, coordinate-preserving handoff to Pallet Town.
route = Path("data/maps/Route1_Frlg/scripts.inc")
text = route.read_text()
pattern = re.compile(
    r"(?ms)^Route1_EventScript_YellowPikachuCapture::\n.*?(?=^Route1_Movement_OakEnterLeft::)"
)
if len(list(pattern.finditer(text))) != 1:
    raise SystemExit("Route 1 capture/escort block signature mismatch")
route_lines = [
    "Route1_EventScript_YellowPikachuCapture::",
    "\tspecial StartYellowPikachuCaptureBattle",
    "\twaitstate",
    "\tlockall",
    "\ttextcolor NPC_TEXT_COLOR_MALE",
    "\tmsgbox PalletTown_Text_OakWhew",
    "\tmsgbox PalletTown_Text_OakComeWithMeYellow",
    "\tclosemessage",
    "\tdelay 20",
    "\tgoto_if_eq VAR_0x8008, 1, Route1_EventScript_YellowEscortWarpRight",
    "\tapplymovement LOCALID_ROUTE1_YELLOW_OAK, Route1_Movement_OakDepartToPalletLeft",
    "\tapplymovement LOCALID_PLAYER, Route1_Movement_PlayerDepartToPalletLeft",
    "\twaitmovement 0",
    "\tsetflag FLAG_HIDE_OAK_IN_PALLET_TOWN",
    "\tremoveobject LOCALID_ROUTE1_YELLOW_OAK",
    "\tsetvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 4",
    "\twarpsilent MAP_PALLET_TOWN, 12, 0",
    "\twaitstate",
    "\tend",
    "",
    "Route1_EventScript_YellowEscortWarpRight::",
    "\tapplymovement LOCALID_ROUTE1_YELLOW_OAK, Route1_Movement_OakMoveBelowPlayerRight",
    "\twaitmovement 0",
    "\tapplymovement LOCALID_ROUTE1_YELLOW_OAK, Route1_Movement_OakDepartToPalletRight",
    "\tapplymovement LOCALID_PLAYER, Route1_Movement_PlayerDepartToPalletRight",
    "\twaitmovement 0",
    "\tsetflag FLAG_HIDE_OAK_IN_PALLET_TOWN",
    "\tremoveobject LOCALID_ROUTE1_YELLOW_OAK",
    "\tsetvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 4",
    "\twarpsilent MAP_PALLET_TOWN, 13, 0",
    "\twaitstate",
    "\tend",
    "",
    "Route1_Movement_OakDepartToPalletLeft::",
    "\twalk_down",
    "\twalk_down",
    "\twalk_down",
    "\tstep_end",
    "",
    "Route1_Movement_PlayerDepartToPalletLeft::",
    "\twalk_down",
    "\twalk_down",
    "\twalk_down",
    "\tstep_end",
    "",
    "Route1_Movement_OakMoveBelowPlayerRight::",
    "\twalk_down",
    "\twalk_right",
    "\twalk_in_place_faster_down",
    "\tstep_end",
    "",
    "Route1_Movement_OakDepartToPalletRight::",
    "\twalk_down",
    "\twalk_down",
    "\tstep_end",
    "",
    "Route1_Movement_PlayerDepartToPalletRight::",
    "\twalk_down",
    "\twalk_down",
    "\tstep_end",
    "",
]
text = pattern.sub("\n".join(route_lines), text, count=1)
route.write_text(text)


# Pallet: preserve Oak's south-facing state and immediately resume the
# existing GBA walk-to-lab choreography after the silent handoff.
pallet = Path("data/maps/PalletTown_Frlg/scripts.inc")
text = pallet.read_text()
pattern = re.compile(
    r"(?ms)^PalletTown_EventScript_YellowOakEscortToLab::\n.*?(?=^PalletTown_EventScript_YellowOakEnterLab::)"
)
if len(list(pattern.finditer(text))) != 1:
    raise SystemExit("Pallet escort setup signature mismatch")
pallet_lines = [
    "PalletTown_EventScript_YellowOakEscortToLab::",
    "\tlockall",
    "\tsetvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 5",
    "\ttextcolor NPC_TEXT_COLOR_MALE",
    "\tclearflag FLAG_HIDE_OAK_IN_PALLET_TOWN",
    "\tgoto_if_eq VAR_0x8008, 1, PalletTown_EventScript_YellowOakEscortRight",
    "\tsetobjectxyperm LOCALID_PALLET_PROF_OAK, 12, 1",
    "\tsetobjectmovementtype LOCALID_PALLET_PROF_OAK, MOVEMENT_TYPE_FACE_DOWN",
    "\taddobject LOCALID_PALLET_PROF_OAK",
    "\tcall PalletTown_EventScript_OakLeadPlayerToLabLeft",
    "\tgoto PalletTown_EventScript_YellowOakEnterLab",
    "",
    "PalletTown_EventScript_YellowOakEscortRight::",
    "\tsetobjectxyperm LOCALID_PALLET_PROF_OAK, 13, 1",
    "\tsetobjectmovementtype LOCALID_PALLET_PROF_OAK, MOVEMENT_TYPE_FACE_DOWN",
    "\taddobject LOCALID_PALLET_PROF_OAK",
    "\tcall PalletTown_EventScript_OakLeadPlayerToLabRight",
    "",
]
text = pattern.sub("\n".join(pallet_lines), text, count=1)
pallet.write_text(text)


lab = Path("data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc")
text = lab.read_text()

# Restore the previously-designed Yellow left-tile initial follower placement.
old = (
    "\tsetflag FLAG_BEAT_RIVAL_IN_OAKS_LAB\n"
    "\tsetflag FLAG_PARTNER_PIKACHU_FOLLOWING\n"
    "\tcallnative UpdateFollowingPokemon\n"
    "\tcallnative RevealFollowingPokemonNow\n"
    "\tdelay 16\n"
)
new = (
    "\tsetflag FLAG_BEAT_RIVAL_IN_OAKS_LAB\n"
    "\tsetflag FLAG_PARTNER_PIKACHU_FOLLOWING\n"
    "\tcallnative UpdateFollowingPokemon\n"
    "\tgetplayerxy VAR_0x8004, VAR_0x8005\n"
    "\tsubvar VAR_0x8004, 1\n"
    "\tsetobjectxy OBJ_EVENT_ID_NPC_FOLLOWER, VAR_0x8004, VAR_0x8005\n"
    "\tturnobject OBJ_EVENT_ID_NPC_FOLLOWER, DIR_EAST\n"
    "\tcallnative RevealFollowingPokemonNow\n"
    "\tdelay 16\n"
)
text = replace_once(text, old, new, "Partner Pikachu initial reveal")

# Yellow-authentic lab dialogue, retaining FRLG/GBA presentation improvements.
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_RivalNoFairWhatAboutMe",
    "PalletTown_ProfessorOaksLab_Text_RivalGoChoosePlayer",
    [
        '\t.string "{RIVAL}: Hey! Gramps! What\\n"',
        '\t.string "about me?$"',
    ],
    "Rival what-about-me dialogue",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakRivalCameEarly",
    "PalletTown_ProfessorOaksLab_Text_OakTakeBallOnTable",
    [
        '\t.string "OAK: Hmm? {RIVAL}?\\n"',
        '\t.string "Why are you here already?\\p"',
        '\t.string "I said for you to come by later...\\p"',
        '\t.string "Ah, whatever! Just wait there.$"',
    ],
    "Oak rival-arrived-early dialogue",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakTakeBallOnTable",
    "PalletTown_ProfessorOaksLab_Text_OakBePatientYellow",
    [
        '\t.string "OAK: Look, {PLAYER}! Do you see that\\n"',
        '\t.string "ball on the table?\\p"',
        '\t.string "It\'s called a POKé BALL. It holds\\n"',
        '\t.string "a POKéMON inside.\\p"',
        '\t.string "You may have it! Go on, take it!$"',
    ],
    "Oak Poké Ball explanation",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakBePatientYellow",
    "PalletTown_ProfessorOaksLab_Text_RivalWantsEevee",
    [
        '\t.string "OAK: Be patient, {RIVAL}.\\n"',
        '\t.string "I\'ll give you one later.$"',
    ],
    "Oak be-patient dialogue",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_RivalSnatchedEevee",
    "PalletTown_ProfessorOaksLab_Text_OakRivalWhatDoing",
    ['\t.string "{RIVAL} snatched the POKéMON!$"'],
    "Rival snatch narration",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakAllowsEevee",
    "PalletTown_ProfessorOaksLab_Text_OakGiveCaughtPikachu",
    [
        '\t.string "OAK: But, I... Oh, all right then.\\p"',
        '\t.string "That POKéMON is yours.\\p"',
        '\t.string "I was going to give you one\\n"',
        '\t.string "anyway...\\p"',
        '\t.string "{PLAYER}, come over here.$"',
    ],
    "Oak allows Eevee dialogue",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakGiveCaughtPikachu",
    "PalletTown_ProfessorOaksLab_Text_OakPikachuCameOut",
    [
        '\t.string "OAK: {PLAYER}, this is the POKéMON I\\n"',
        '\t.string "caught earlier.\\p"',
        '\t.string "You can have it. I caught it in\\n"',
        '\t.string "the wild, and it\'s not tame yet.$"',
    ],
    "Oak gives caught Pikachu dialogue",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakPikachuCameOut",
    "PalletTown_ProfessorOaksLab_Text_OakPikachuDislikesBalls",
    ['\t.string "OAK: What?$"'],
    "Oak Pikachu escape reaction",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakPikachuDislikesBalls",
    "PalletTown_ProfessorOaksLab_Text_OakReadyForStarter",
    [
        '\t.string "OAK: Would you look at that!\\p"',
        '\t.string "It\'s odd, but it appears that your\\n"',
        '\t.string "PIKACHU dislikes POKé BALLS.\\p"',
        '\t.string "You should just keep it with you.\\p"',
        '\t.string "That should make it happy!\\p"',
        '\t.string "You can talk to it and see how it\\n"',
        '\t.string "feels about you.$"',
    ],
    "Oak Partner Pikachu explanation",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakBattleMonForItToGrow",
    "PalletTown_ProfessorOaksLab_Text_OakHaveSomethingForMe",
    [
        '\t.string "OAK: You should talk to PIKACHU and\\n"',
        '\t.string "see how it feels.$"',
    ],
    "Oak post-opening Partner advice",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_RivalWhatDidYouCallMeFor",
    "PalletTown_ProfessorOaksLab_Text_RivalLeaveItToMeGramps",
    [
        '\t.string "{RIVAL}: Gramps, my POKéMON has\\n"',
        '\t.string "grown stronger! Check it out!$"',
    ],
    "Rival parcel-return boast",
)
text = replace_section(
    text,
    "PalletTown_ProfessorOaksLab_Text_OakHaveRequestForYouTwo",
    "PalletTown_ProfessorOaksLab_Text_OakPokedexOnDesk",
    [
        '\t.string "OAK: Ah, {RIVAL}, good timing!\\p"',
        '\t.string "I needed to ask both of you to do\\n"',
        '\t.string "something for me.$"',
    ],
    "Oak Pokédex request transition",
)

lab.write_text(text)
