// Golden Yellow development overlay for the expansion debug menu.
// Keep the upstream implementation intact in debug_base.inc and replace only
// the public main-menu entry point so project-specific checkpoints stay isolated.
#define Debug_ShowMainMenu Debug_ShowMainMenu_Base
#include "debug_base.inc"
#undef Debug_ShowMainMenu

#include "golden_yellow_debug.h"

struct GoldenYellowDebugMenuSelection
{
    enum GoldenYellowDebugCheckpoint checkpoint;
    enum GoldenYellowDebugRivalPath rivalPath;
};

static void DebugAction_GoldenYellow_ApplyCheckpoint(u8 taskId, const void *params)
{
    const struct GoldenYellowDebugMenuSelection *selection = params;

    Debug_DestroyMenu_Full(taskId);
    GoldenYellowDebug_ApplyCheckpoint(selection->checkpoint, selection->rivalPath);
    ScriptContext_Stop();
}

#define GY_SELECTION(name, cp, path) \
    static const struct GoldenYellowDebugMenuSelection name = { cp, path }

GY_SELECTION(sGyBeforeWatch, GY_DEBUG_CP_BEFORE_TRAINER_WATCH, GY_DEBUG_RIVAL_UNSET);
GY_SELECTION(sGyBeforeRoute1Oak, GY_DEBUG_CP_BEFORE_ROUTE1_OAK, GY_DEBUG_RIVAL_UNSET);
GY_SELECTION(sGyOakLab, GY_DEBUG_CP_OAK_LAB_RIVAL, GY_DEBUG_RIVAL_UNSET);
GY_SELECTION(sGyRoute22Early, GY_DEBUG_CP_ROUTE22_EARLY, GY_DEBUG_RIVAL_FLAREON);
GY_SELECTION(sGyCerulean, GY_DEBUG_CP_CERULEAN_RIVAL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGySSAnne, GY_DEBUG_CP_SS_ANNE_RIVAL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyTowerJolteon, GY_DEBUG_CP_POKEMON_TOWER_RIVAL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyTowerFlareon, GY_DEBUG_CP_POKEMON_TOWER_RIVAL, GY_DEBUG_RIVAL_FLAREON);
GY_SELECTION(sGyTowerVaporeon, GY_DEBUG_CP_POKEMON_TOWER_RIVAL, GY_DEBUG_RIVAL_VAPOREON);
GY_SELECTION(sGySilphJolteon, GY_DEBUG_CP_SILPH_RIVAL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGySilphFlareon, GY_DEBUG_CP_SILPH_RIVAL, GY_DEBUG_RIVAL_FLAREON);
GY_SELECTION(sGySilphVaporeon, GY_DEBUG_CP_SILPH_RIVAL, GY_DEBUG_RIVAL_VAPOREON);
GY_SELECTION(sGyRoute22LateJolteon, GY_DEBUG_CP_ROUTE22_LATE_RIVAL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyRoute22LateFlareon, GY_DEBUG_CP_ROUTE22_LATE_RIVAL, GY_DEBUG_RIVAL_FLAREON);
GY_SELECTION(sGyRoute22LateVaporeon, GY_DEBUG_CP_ROUTE22_LATE_RIVAL, GY_DEBUG_RIVAL_VAPOREON);
GY_SELECTION(sGyChampionJolteon, GY_DEBUG_CP_CHAMPION_RIVAL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyChampionFlareon, GY_DEBUG_CP_CHAMPION_RIVAL, GY_DEBUG_RIVAL_FLAREON);
GY_SELECTION(sGyChampionVaporeon, GY_DEBUG_CP_CHAMPION_RIVAL, GY_DEBUG_RIVAL_VAPOREON);

// D1 early-Kanto main-line selections use Jolteon as the deterministic default
// Rival path. Rival-specific checkpoints remain available for all three paths.
GY_SELECTION(sGyBeforeTeachyTV, GY_DEBUG_CP_BEFORE_TEACHY_TV, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBeforePewterGuide, GY_DEBUG_CP_BEFORE_PEWTER_GUIDE, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBeforeJigglypuff, GY_DEBUG_CP_BEFORE_JIGGLYPUFF, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBeforeBrock, GY_DEBUG_CP_BEFORE_BROCK, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBeforeRunningShoes, GY_DEBUG_CP_BEFORE_RUNNING_SHOES, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyMtMoonFossil, GY_DEBUG_CP_MT_MOON_FOSSIL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyMtMoonJessieJames, GY_DEBUG_CP_MT_MOON_JESSIE_JAMES, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBeforeMisty, GY_DEBUG_CP_BEFORE_MISTY, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyNuggetBridgeRocket, GY_DEBUG_CP_NUGGET_BRIDGE_ROCKET, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBeforeBill, GY_DEBUG_CP_BEFORE_BILL, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBulbasaurGift, GY_DEBUG_CP_BULBASAUR_GIFT, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyCharmanderGift, GY_DEBUG_CP_CHARMANDER_GIFT, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGySSAnneCaptain, GY_DEBUG_CP_SS_ANNE_CAPTAIN, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGyBeforeLtSurge, GY_DEBUG_CP_BEFORE_LT_SURGE, GY_DEBUG_RIVAL_JOLTEON);
GY_SELECTION(sGySquirtleGift, GY_DEBUG_CP_SQUIRTLE_GIFT, GY_DEBUG_RIVAL_JOLTEON);

#undef GY_SELECTION

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Tower[] =
{
    { COMPOUND_STRING("Jolteon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyTowerJolteon },
    { COMPOUND_STRING("Flareon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyTowerFlareon },
    { COMPOUND_STRING("Vaporeon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyTowerVaporeon },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Silph[] =
{
    { COMPOUND_STRING("Jolteon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySilphJolteon },
    { COMPOUND_STRING("Flareon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySilphFlareon },
    { COMPOUND_STRING("Vaporeon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySilphVaporeon },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Route22Late[] =
{
    { COMPOUND_STRING("Jolteon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyRoute22LateJolteon },
    { COMPOUND_STRING("Flareon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyRoute22LateFlareon },
    { COMPOUND_STRING("Vaporeon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyRoute22LateVaporeon },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Champion[] =
{
    { COMPOUND_STRING("Jolteon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyChampionJolteon },
    { COMPOUND_STRING("Flareon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyChampionFlareon },
    { COMPOUND_STRING("Vaporeon path"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyChampionVaporeon },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_CampaignOpeningPewter[] =
{
    { COMPOUND_STRING("Before Trainer Watch"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeWatch },
    { COMPOUND_STRING("Before Route 1 Oak"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeRoute1Oak },
    { COMPOUND_STRING("Oak Lab Rival"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyOakLab },
    { COMPOUND_STRING("Route 22 - Early"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyRoute22Early },
    { COMPOUND_STRING("Before Teachy TV"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeTeachyTV },
    { COMPOUND_STRING("Before Pewter Guide"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforePewterGuide },
    { COMPOUND_STRING("Before Jigglypuff"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeJigglypuff },
    { COMPOUND_STRING("Before Brock"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeBrock },
    { COMPOUND_STRING("Before Running Shoes"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeRunningShoes },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_CampaignMtMoonCerulean[] =
{
    { COMPOUND_STRING("Mt. Moon Fossil"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyMtMoonFossil },
    { COMPOUND_STRING("Mt. Moon Jessie/James"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyMtMoonJessieJames },
    { COMPOUND_STRING("Before Misty"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeMisty },
    { COMPOUND_STRING("Cerulean Rival"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyCerulean },
    { COMPOUND_STRING("Nugget Bridge Rocket"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyNuggetBridgeRocket },
    { COMPOUND_STRING("Charmander Gift"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyCharmanderGift },
    { COMPOUND_STRING("Before Bill"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeBill },
    { COMPOUND_STRING("Bulbasaur Gift"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBulbasaurGift },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_CampaignVermilion[] =
{
    { COMPOUND_STRING("S.S. Anne Rival"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySSAnne },
    { COMPOUND_STRING("S.S. Anne Captain"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySSAnneCaptain },
    { COMPOUND_STRING("Before Lt. Surge"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeLtSurge },
    { COMPOUND_STRING("Squirtle Gift"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySquirtleGift },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Campaign[] =
{
    { COMPOUND_STRING("Opening & Pewter…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_CampaignOpeningPewter },
    { COMPOUND_STRING("Mt. Moon & Cerulean…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_CampaignMtMoonCerulean },
    { COMPOUND_STRING("Vermilion…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_CampaignVermilion },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Rivals[] =
{
    { COMPOUND_STRING("Oak Lab"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyOakLab },
    { COMPOUND_STRING("Route 22 - Early"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyRoute22Early },
    { COMPOUND_STRING("Cerulean"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyCerulean },
    { COMPOUND_STRING("S.S. Anne"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySSAnne },
    { COMPOUND_STRING("Pokemon Tower…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Tower },
    { COMPOUND_STRING("Silph Co.…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Silph },
    { COMPOUND_STRING("Route 22 - Late…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Route22Late },
    { COMPOUND_STRING("Champion…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Champion },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_YellowOpening[] =
{
    { COMPOUND_STRING("Before Trainer Watch"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeWatch },
    { COMPOUND_STRING("Before Route 1 Oak"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeRoute1Oak },
    { COMPOUND_STRING("Oak Lab"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyOakLab },
    { COMPOUND_STRING("Before Teachy TV"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeTeachyTV },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Gifts[] =
{
    { COMPOUND_STRING("Bulbasaur"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBulbasaurGift },
    { COMPOUND_STRING("Charmander"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyCharmanderGift },
    { COMPOUND_STRING("Squirtle"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGySquirtleGift },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_JessieJames[] =
{
    { COMPOUND_STRING("Mt. Moon"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyMtMoonJessieJames },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_YellowEvents[] =
{
    { COMPOUND_STRING("Partner & Opening…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_YellowOpening },
    { COMPOUND_STRING("Gift Pokemon…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Gifts },
    { COMPOUND_STRING("Jessie & James…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_JessieJames },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Gyms[] =
{
    { COMPOUND_STRING("Brock"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeBrock },
    { COMPOUND_STRING("Misty"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeMisty },
    { COMPOUND_STRING("Lt. Surge"), DebugAction_GoldenYellow_ApplyCheckpoint, &sGyBeforeLtSurge },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_GoldenYellow_Checkpoints[] =
{
    { COMPOUND_STRING("Campaign Progression…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Campaign },
    { COMPOUND_STRING("Rival Battles…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Rivals },
    { COMPOUND_STRING("Yellow Events…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_YellowEvents },
    { COMPOUND_STRING("Gym Leaders…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Gyms },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Main_GoldenYellow[] =
{
    { COMPOUND_STRING("Utilities…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_Utilities },
    { COMPOUND_STRING("Golden Yellow…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_GoldenYellow_Checkpoints },
    { COMPOUND_STRING("PC/Bag…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_PCBag },
    { COMPOUND_STRING("Party…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_Party },
    { COMPOUND_STRING("Give X…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_Give },
    { COMPOUND_STRING("Player…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_Player },
    { COMPOUND_STRING("Scripts…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_Scripts },
    { COMPOUND_STRING("Trainers…"), DebugAction_OpenSubMenuTrainers, sDebugMenu_Actions_Trainers },
    { COMPOUND_STRING("Flags & Vars…"), DebugAction_OpenSubMenuFlagsVars, sDebugMenu_Actions_Flags },
    { COMPOUND_STRING("Sound…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_Sound },
    { COMPOUND_STRING("ROM Info…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_ROMInfo2 },
    { COMPOUND_STRING("Cancel"), DebugAction_Cancel },
    { NULL }
};

void Debug_ShowMainMenu(void);

void Debug_ShowMainMenu(void)
{
    sDebugMenuListData = AllocZeroed(sizeof(*sDebugMenuListData));
    sDebugMenuListData->menuType = DEBUG_BASIC_MENU;
    Debug_ShowMenu(DebugTask_HandleMenuInput_General, sDebugMenu_Actions_Main_GoldenYellow);
}
