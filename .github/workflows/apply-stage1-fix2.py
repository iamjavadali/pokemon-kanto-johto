from pathlib import Path
import re


def read(path):
    return Path(path).read_text()


def write(path, text):
    Path(path).write_text(text)


def replace_once(path, old, new):
    text = read(path)
    count = text.count(old)
    if count != 1:
        raise SystemExit(f"{path}: expected exactly 1 exact match, found {count}")
    write(path, text.replace(old, new, 1))


def replace_regex_once(path, pattern, replacement):
    text = read(path)
    new_text, count = re.subn(pattern, lambda _match: replacement, text, count=1, flags=re.S)
    if count != 1:
        raise SystemExit(f"{path}: expected exactly 1 regex match, found {count}: {pattern[:80]}")
    write(path, new_text)


oak = "src/oak_speech.c"

replace_regex_once(
    oak,
    r'''static void Task_OakSpeech_AskPlayerGender\(u8 taskId\)\n\{.*?\n\}\n\nstatic void Task_OakSpeech_ShowGenderOptions''',
    '''static void Task_OakSpeech_AskPlayerGender(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (tTrainerPicFadeState != 0)
    {
        if (tTimer != 0)
        {
            tTimer--;
        }
        else
        {
            tTrainerPicPosX = 0;
            gSpriteCoordOffsetX = 0;
            ChangeBgX(2, 0, BG_COORD_SET);
            ClearTrainerPic();
            gSaveBlock2Ptr->playerGender = MALE;
            LoadTrainerPic(MALE_PLAYER_PIC, 0);
            CreateFadeOutTask(taskId, 2);
            OakSpeechPrintMessage(gOakSpeech_Text_AskPlayerGender, sOakSpeechResources->textSpeed, FALSE);
            gTasks[taskId].func = Task_OakSpeech_ShowGenderOptions;
        }
    }
}

static void Task_OakSpeech_ShowGenderOptions'''
)

replace_regex_once(
    oak,
    r'''static void Task_OakSpeech_HandleGenderInput\(u8 taskId\)\n\{.*?\n\}\n\nstatic void Task_OakSpeech_ClearGenderWindows''',
    '''static void Task_OakSpeech_HandleGenderInput(u8 taskId)
{
    u8 cursorPos;
    s8 input = Menu_ProcessInputNoWrap();

    cursorPos = Menu_GetCursorPos();
    if (cursorPos != gSaveBlock2Ptr->playerGender)
    {
        gSaveBlock2Ptr->playerGender = cursorPos;
        ClearTrainerPic();
        if (cursorPos == MALE)
            LoadTrainerPic(MALE_PLAYER_PIC, 0);
        else
            LoadTrainerPic(FEMALE_PLAYER_PIC, 0);
    }

    switch (input)
    {
    case 0: // BOY
        gSaveBlock2Ptr->playerGender = MALE;
        break;
    case 1: // GIRL
        gSaveBlock2Ptr->playerGender = FEMALE;
        break;
    case MENU_B_PRESSED:
    case MENU_NOTHING_CHOSEN:
        return;
    }
    gTasks[taskId].func = Task_OakSpeech_ClearGenderWindows;
}

static void Task_OakSpeech_ClearGenderWindows'''
)

replace_regex_once(
    oak,
    r'''static void Task_OakSpeech_ClearGenderWindows\(u8 taskId\)\n\{.*?\n\}\n\nstatic void Task_OakSpeech_LoadPlayerPic''',
    '''static void Task_OakSpeech_ClearGenderWindows(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ClearStdWindowAndFrameToTransparent(tMenuWindowId, TRUE);
    RemoveWindow(tMenuWindowId);
    tMenuWindowId = WIN_INTRO_TEXTBOX;
    ClearDialogWindowAndFrame(tMenuWindowId, TRUE);
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 30, 20);
    CopyBgTilemapBufferToVram(0);
    tTimer = 32;
    gTasks[taskId].func = Task_OakSpeech_YourNameWhatIsIt;
}

static void Task_OakSpeech_LoadPlayerPic'''
)

replace_regex_once(
    oak,
    r'''static void Task_OakSpeech_DoNamingScreen\(u8 taskId\)\n\{.*?\n\}\n\nstatic void Task_OakSpeech_ConfirmName''',
    '''static void Task_OakSpeech_DoNamingScreen(u8 taskId)
{
    u8 i;

    if (!gPaletteFade.active)
    {
        if (sOakSpeechResources->hasPlayerBeenNamed == FALSE)
        {
            for (i = 0; i < PLAYER_NAME_LENGTH + 1; i++)
                gSaveBlock2Ptr->playerName[i] = EOS;
            DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnFromNamingScreen);
        }
        else
        {
            ClearStdWindowAndFrameToTransparent(gTasks[taskId].tMenuWindowId, TRUE);
            RemoveWindow(gTasks[taskId].tMenuWindowId);
            for (i = 0; i < PLAYER_NAME_LENGTH + 1; i++)
                gSaveBlock1Ptr->rivalName[i] = EOS;
            DoNamingScreen(NAMING_SCREEN_RIVAL, gSaveBlock1Ptr->rivalName, 0, 0, 0, CB2_ReturnFromNamingScreen);
        }
        DestroyPikachuOrPlatformSprites(taskId, SPRITE_TYPE_PLATFORM);
        FreeAllWindowBuffers();
    }
}

static void Task_OakSpeech_ConfirmName'''
)

replace_regex_once(
    oak,
    r'''static void GetDefaultName\(u8 hasPlayerBeenNamed, u8 nameChoice\)\n\{.*?\n\}\n\n#undef tSpriteTimer''',
    '''static void GetDefaultName(u8 hasPlayerBeenNamed, u8 nameChoice)
{
    const u8 *src;
    u8 *dest;
    u8 i;

    if (hasPlayerBeenNamed == FALSE)
    {
        if (gSaveBlock2Ptr->playerGender == MALE)
            src = sMaleNameChoices[nameChoice];
        else
            src = sFemaleNameChoices[nameChoice];
        dest = gSaveBlock2Ptr->playerName;
    }
    else
    {
        src = sRivalNameChoices[nameChoice];
        dest = gSaveBlock1Ptr->rivalName;
    }
    for (i = 0; i < PLAYER_NAME_LENGTH && src[i] != EOS; i++)
        dest[i] = src[i];
    for (; i < PLAYER_NAME_LENGTH + 1; i++)
        dest[i] = EOS;
}

#undef tSpriteTimer'''
)

map_path = "data/maps/PalletTown_Frlg/map.json"
replace_once(
    map_path,
    '''      "x": 12,
      "y": 1,
      "elevation": 3,
      "var": "VAR_MAP_SCENE_PALLET_TOWN_OAK",
      "var_value": "0",
      "script": "PalletTown_EventScript_OakTriggerLeft"''',
    '''      "x": 12,
      "y": 0,
      "elevation": 3,
      "var": "VAR_MAP_SCENE_PALLET_TOWN_OAK",
      "var_value": "0",
      "script": "PalletTown_EventScript_OakTriggerLeft"'''
)
replace_once(
    map_path,
    '''      "x": 13,
      "y": 1,
      "elevation": 3,
      "var": "VAR_MAP_SCENE_PALLET_TOWN_OAK",
      "var_value": "0",
      "script": "PalletTown_EventScript_OakTriggerRight"''',
    '''      "x": 13,
      "y": 0,
      "elevation": 3,
      "var": "VAR_MAP_SCENE_PALLET_TOWN_OAK",
      "var_value": "0",
      "script": "PalletTown_EventScript_OakTriggerRight"'''
)

pallet = "data/maps/PalletTown_Frlg/scripts.inc"
replace_regex_once(
    pallet,
    r'''PalletTown_EventScript_OakTrigger::\n.*?\nPalletTown_EventScript_OakEnterLeft::''',
    '''PalletTown_EventScript_OakTrigger::
\tfamechecker FAMECHECKER_OAK, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
\ttextcolor NPC_TEXT_COLOR_MALE
\tdelay 30
\tplaybgm MUS_RG_OAK, 0
\tmessage PalletTown_Text_OakDontGoOut
\twaitmessage
\tdelay 85
\tclosemessage
\tapplymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
\twaitmovement 0
\tplayse SE_PIN
\tapplymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
\twaitmovement 0
\tdelay 30
\taddobject LOCALID_PALLET_PROF_OAK
\tcall_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_OakEnterLeft
\tcall_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_OakEnterRight
\tdelay 30
\tmsgbox PalletTown_Text_OakGrassUnsafeNeedMon
\tclosemessage
\tcall_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_FaceYellowGrassLeft
\tcall_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_FaceYellowGrassRight
\tspecial StartYellowPikachuCaptureBattle
\twaitstate
\tlockall
\ttextcolor NPC_TEXT_COLOR_MALE
\tmsgbox PalletTown_Text_OakWhew
\tmsgbox PalletTown_Text_OakComeWithMeYellow
\tclosemessage
\tdelay 20
\tcall_if_eq VAR_TEMP_1, 0, PalletTown_EventScript_OakLeadPlayerToLabLeft
\tcall_if_eq VAR_TEMP_1, 1, PalletTown_EventScript_OakLeadPlayerToLabRight
\topendoor 16, 13
\twaitdooranim
\tapplymovement LOCALID_PALLET_PROF_OAK, PalletTown_Movement_OakEnterLab
\tapplymovement LOCALID_PLAYER, PalletTown_Movement_PlayerEnterLab
\twaitmovement 0
\tclosedoor 16, 13
\twaitdooranim
\tsetvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 1
\tclearflag FLAG_HIDE_OAK_IN_HIS_LAB
\tsetvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 1
\tsetflag FLAG_HIDE_OAK_IN_PALLET_TOWN
\tsetflag FLAG_HIDE_BULBASAUR_BALL
\tclearflag FLAG_HIDE_SQUIRTLE_BALL
\tsetflag FLAG_HIDE_CHARMANDER_BALL
\tclearflag FLAG_HIDE_RIVAL_IN_LAB
\tsetflag FLAG_DONT_TRANSITION_MUSIC
\twarp MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 12
\twaitstate
\treleaseall
\tend

PalletTown_EventScript_FaceYellowGrassLeft::
\tapplymovement LOCALID_PLAYER, Common_Movement_FaceRight
\tapplymovement LOCALID_PALLET_PROF_OAK, Common_Movement_FaceRight
\twaitmovement 0
\treturn

PalletTown_EventScript_FaceYellowGrassRight::
\tapplymovement LOCALID_PLAYER, Common_Movement_FaceLeft
\tapplymovement LOCALID_PALLET_PROF_OAK, Common_Movement_FaceLeft
\twaitmovement 0
\treturn

PalletTown_EventScript_OakEnterLeft::'''
)

replace_once(pallet, '''PalletTown_Movement_OakEnterLeft::
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\tstep_end''', '''PalletTown_Movement_OakEnterLeft::
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\twalk_up
\tstep_end''')
replace_once(pallet, '''PalletTown_Movement_OakEnterRight::
\twalk_right
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\tstep_end''', '''PalletTown_Movement_OakEnterRight::
\twalk_right
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\twalk_right
\twalk_up
\twalk_up
\twalk_up
\tstep_end''')
replace_once(pallet, '''PalletTown_Movement_OakWalkToLabLeft::
\twalk_down
\twalk_to_lab''', '''PalletTown_Movement_OakWalkToLabLeft::
\twalk_down
\twalk_down
\twalk_to_lab''')
replace_once(pallet, '''PalletTown_Movement_OakWalkToLabRight::
\twalk_down
\twalk_left
\twalk_to_lab''', '''PalletTown_Movement_OakWalkToLabRight::
\twalk_down
\twalk_down
\twalk_left
\twalk_to_lab''')
replace_once(pallet, '''PalletTown_Movement_PlayerWalkToLabLeft::
\twalk_down
\twalk_down
\twalk_to_lab''', '''PalletTown_Movement_PlayerWalkToLabLeft::
\twalk_down
\twalk_down
\twalk_down
\twalk_to_lab''')
replace_once(pallet, '''PalletTown_Movement_PlayerWalkToLabRight::
\twalk_down
\twalk_down
\twalk_left
\twalk_to_lab''', '''PalletTown_Movement_PlayerWalkToLabRight::
\twalk_down
\twalk_down
\twalk_down
\twalk_left
\twalk_to_lab''')

replace_regex_once(
    pallet,
    r'''PalletTown_Text_OakGrassUnsafeNeedMon::\n.*?\nPalletTown_Text_RaisingMonsToo::''',
    '''PalletTown_Text_OakGrassUnsafeNeedMon::
\t.string "OAK: That was close!\\n"
\t.string "Wild POKéMON live in tall grass!$"

PalletTown_Text_OakWhew::
\t.string "OAK: Whew...$"

PalletTown_Text_OakComeWithMeYellow::
\t.string "OAK: A POKéMON can appear at any\\n"
\t.string "time in tall grass.\\p"
\t.string "You need a POKéMON of your own\\n"
\t.string "for protection.\\p"
\t.string "I know! Come with me!$"

PalletTown_Text_RaisingMonsToo::'''
)

battle_h = "include/constants/battle.h"
replace_once(battle_h, "#define BATTLE_TYPE_30                 (1 << 30)", "#define BATTLE_TYPE_YELLOW_PIKACHU     (1 << 30)")

battle_setup = "src/battle_setup.c"
replace_regex_once(
    battle_setup,
    r'''void StartOldManTutorialBattle\(void\)\n\{\n    CreateMaleMon\(&gParties\[B_TRAINER_OPPONENT_A\]\[0\], SPECIES_WEEDLE, 5\);\n    LockPlayerFieldControls\(\);\n    gMain\.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;\n    gBattleTypeFlags = BATTLE_TYPE_CATCH_TUTORIAL;\n    CreateBattleStartTask\(B_TRANSITION_SLICE, 0\);\n\}''',
    '''void StartOldManTutorialBattle(void)
{
    CreateMaleMon(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_WEEDLE, 5);
    LockPlayerFieldControls();
    gMain.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
    gBattleTypeFlags = BATTLE_TYPE_CATCH_TUTORIAL;
    CreateBattleStartTask(B_TRANSITION_SLICE, 0);
}

void StartYellowPikachuCaptureBattle(void)
{
    CreateMaleMon(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_PIKACHU, 5);
    LockPlayerFieldControls();
    gMain.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
    gBattleTypeFlags = BATTLE_TYPE_CATCH_TUTORIAL | BATTLE_TYPE_YELLOW_PIKACHU;
    CreateBattleStartTask(B_TRANSITION_SLICE, 0);
}'''
)

specials = "data/specials.inc"
replace_once(specials, "\tdef_special StartOldManTutorialBattle\n", "\tdef_special StartOldManTutorialBattle\n\tdef_special StartYellowPikachuCaptureBattle, waitstate=1\n")

controller = "src/battle_controller_oak_old_man.c"
replace_once(controller, 'static const u8 gText_WhatWillOldManDo[] = _("What will the\\nold man do?");', 'static const u8 gText_WhatWillOldManDo[] = _("What will the\\nold man do?");\nstatic const u8 sText_WhatWillOakDo[] = _("What will\\nOAK do?");')
replace_regex_once(
    controller,
    r'''static void OakOldManHandleDrawTrainerPic\(enum BattlerId battler\)\n\{.*?\n\}\n\nstatic void OakOldManHandleTrainerSlide''',
    '''static void OakOldManHandleDrawTrainerPic(enum BattlerId battler)
{
    enum TrainerPicID trainerPicId;

    if (gBattleTypeFlags & BATTLE_TYPE_YELLOW_PIKACHU)
    {
        trainerPicId = TRAINER_PIC_PROFESSOR_OAK_FRLG;
        BtlController_HandleDrawTrainerPic(battler, trainerPicId, TRUE, 80, 80, 30);
        return;
    }

    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
        trainerPicId = GetPlayerTrainerPic(gSaveBlock2Ptr->playerGender, GAME_VERSION);
    else
        trainerPicId = TRAINER_PIC_OLD_MAN;

    BtlController_HandleDrawTrainerPic(battler, trainerPicId, FALSE, 80, (8 - GetTrainerBackPicCoords(trainerPicId)->size) * 4 + 80, 30);
}

static void OakOldManHandleTrainerSlide'''
)
replace_regex_once(
    controller,
    r'''static void OakOldManHandleTrainerSlide\(enum BattlerId battler\)\n\{.*?\n\}\n\nstatic void OakOldManHandlePrintString''',
    '''static void OakOldManHandleTrainerSlide(enum BattlerId battler)
{
    enum TrainerPicID trainerPicId;

    if (gBattleTypeFlags & BATTLE_TYPE_YELLOW_PIKACHU)
    {
        OakOldManBufferExecCompleted(battler);
        return;
    }

    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
        trainerPicId = GetPlayerTrainerPic(gSaveBlock2Ptr->playerGender, GAME_VERSION);
    else
        trainerPicId = TRAINER_PIC_OLD_MAN;

    BtlController_HandleTrainerSlide(battler, trainerPicId);
}

static void OakOldManHandlePrintString'''
)
replace_once(controller, '''    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
    {
        PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, battler, gBattlerPartyIndexes[battler]);
        BattleStringExpandPlaceholdersToDisplayedString(gText_WhatWillPkmnDo);
    }
    else
        BattleStringExpandPlaceholdersToDisplayedString(gText_WhatWillOldManDo);''', '''    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
    {
        PREPARE_MON_NICK_BUFFER(gBattleTextBuff1, battler, gBattlerPartyIndexes[battler]);
        BattleStringExpandPlaceholdersToDisplayedString(gText_WhatWillPkmnDo);
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_YELLOW_PIKACHU)
        BattleStringExpandPlaceholdersToDisplayedString(sText_WhatWillOakDo);
    else
        BattleStringExpandPlaceholdersToDisplayedString(gText_WhatWillOldManDo);''')

species = "src/data/pokemon/species_info/gen_1_families.h"
text = read(species)
start = text.find("[SPECIES_PIKACHU] =")
if start == -1:
    raise SystemExit("regular Pikachu species block not found")
end = text.find("\n    [SPECIES_", start + 1)
if end == -1:
    raise SystemExit("end of regular Pikachu species block not found")
block = text[start:end]
if ".baseDefense   = PIKACHU_DEFENSE," not in block:
    raise SystemExit("regular Pikachu defense macro not found")
block = block.replace(".baseDefense   = PIKACHU_DEFENSE,", ".baseDefense   = 30,", 1)
spdef_pattern = r"\.baseSpDefense = P_UPDATED_STATS >= GEN_6 \|\| P_UPDATED_STATS == GEN_1 \? 50 : 40,"
block, n = re.subn(spdef_pattern, ".baseSpDefense = 50,", block, count=1)
if n != 1:
    raise SystemExit("regular Pikachu special defense expression not found")
if ".growthRate = GROWTH_MEDIUM_FAST," not in block:
    raise SystemExit("regular Pikachu no longer uses Medium Fast growth")
text = text[:start] + block + text[end:]
write(species, text)

learnsets = read("src/data/pokemon/level_up_learnsets/gen_1.h")
yellow_moves = [
    "LEVEL_UP_MOVE( 1, MOVE_THUNDER_SHOCK)",
    "LEVEL_UP_MOVE( 1, MOVE_GROWL)",
    "LEVEL_UP_MOVE( 6, MOVE_TAIL_WHIP)",
    "LEVEL_UP_MOVE( 8, MOVE_THUNDER_WAVE)",
    "LEVEL_UP_MOVE(11, MOVE_QUICK_ATTACK)",
    "LEVEL_UP_MOVE(15, MOVE_DOUBLE_TEAM)",
    "LEVEL_UP_MOVE(20, MOVE_SLAM)",
    "LEVEL_UP_MOVE(26, MOVE_THUNDERBOLT)",
    "LEVEL_UP_MOVE(33, MOVE_AGILITY)",
    "LEVEL_UP_MOVE(41, MOVE_THUNDER)",
    "LEVEL_UP_MOVE(50, MOVE_LIGHT_SCREEN)",
]
pikachu_moves_start = learnsets.find("static const struct LevelUpMove sPikachuLevelUpLearnset[]")
pikachu_moves_end = learnsets.find("LEVEL_UP_END", pikachu_moves_start)
pikachu_moves = learnsets[pikachu_moves_start:pikachu_moves_end]
for move in yellow_moves:
    if move not in pikachu_moves:
        raise SystemExit(f"Yellow Pikachu learnset guard failed: {move}")

if "Random() % ARRAY_COUNT(sMaleNameChoices)" in read(oak) or "Random() % ARRAY_COUNT(sFemaleNameChoices)" in read(oak):
    raise SystemExit("player preset naming still randomizes")
if "Menu_GetCursorPos()" not in read(oak):
    raise SystemExit("live gender preview cursor tracking missing")
if "StartYellowPikachuCaptureBattle" not in read(pallet):
    raise SystemExit("Yellow Pikachu battle capture special not called")
oak_trigger = read(pallet).split("PalletTown_EventScript_OakTrigger::", 1)[1].split("PalletTown_EventScript_OakEnterLeft::", 1)[0]
if "SE_BALL_THROW" in oak_trigger:
    raise SystemExit("obsolete fake overworld capture still active")
if "BATTLE_TYPE_YELLOW_PIKACHU" not in read(controller):
    raise SystemExit("Oak capture controller mode missing")
if "TRAINER_PIC_PROFESSOR_OAK_FRLG" not in read(controller):
    raise SystemExit("Oak capture trainer graphic missing")

print("Stage 1 Fix 2 guarded patch applied successfully.")
