#include "global.h"
#include "golden_yellow_debug.h"
#include "event_data.h"
#include "field_screen_effect.h"
#include "follower_npc.h"
#include "item_menu.h"
#include "money.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "rtc.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "constants/vars.h"

#define VAR_YELLOW_RIVAL_EEVEE_STATE 0x40FD

extern const u8 EventScript_ResetAllMapFlagsFrlg[];

struct GoldenYellowDebugMon
{
    enum Species species;
    u8 level;
};

struct GoldenYellowDebugPartnerProfile
{
    u8 level;
    enum Move moves[MAX_MON_MOVES];
};

struct GoldenYellowDebugWarp
{
    u16 map;
    s16 x;
    s16 y;
};

static const struct GoldenYellowDebugPartnerProfile sPartnerOakLab =
{
    .level = 5,
    .moves = { MOVE_THUNDER_SHOCK, MOVE_GROWL, MOVE_NONE, MOVE_NONE },
};

static const struct GoldenYellowDebugPartnerProfile sPartnerRoute22 =
{
    .level = 10,
    .moves = { MOVE_THUNDER_SHOCK, MOVE_GROWL, MOVE_TAIL_WHIP, MOVE_THUNDER_WAVE },
};

static const struct GoldenYellowDebugPartnerProfile sPartnerCerulean =
{
    .level = 20,
    .moves = { MOVE_THUNDER_SHOCK, MOVE_QUICK_ATTACK, MOVE_DOUBLE_TEAM, MOVE_THUNDER_WAVE },
};

static const struct GoldenYellowDebugPartnerProfile sPartnerSSAnne =
{
    .level = 24,
    .moves = { MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK, MOVE_DOUBLE_TEAM, MOVE_THUNDER_WAVE },
};

static const struct GoldenYellowDebugPartnerProfile sPartnerTower =
{
    .level = 30,
    .moves = { MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK, MOVE_DOUBLE_TEAM, MOVE_SLAM },
};

static const struct GoldenYellowDebugPartnerProfile sPartnerSilph =
{
    .level = 42,
    .moves = { MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK, MOVE_DOUBLE_TEAM, MOVE_SLAM },
};

static const struct GoldenYellowDebugPartnerProfile sPartnerLateRoute22 =
{
    .level = 55,
    .moves = { MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK, MOVE_DOUBLE_TEAM, MOVE_SLAM },
};

static const struct GoldenYellowDebugPartnerProfile sPartnerChampion =
{
    .level = 66,
    .moves = { MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK, MOVE_DOUBLE_TEAM, MOVE_SLAM },
};

static const struct GoldenYellowDebugMon sPartyRoute22[] =
{
    { SPECIES_NIDORAN_M, 8 },
};

static const struct GoldenYellowDebugMon sPartyCerulean[] =
{
    { SPECIES_BUTTERFREE, 18 },
    { SPECIES_NIDORINO, 18 },
};

static const struct GoldenYellowDebugMon sPartySSAnne[] =
{
    { SPECIES_BUTTERFREE, 22 },
    { SPECIES_NIDORINO, 22 },
};

static const struct GoldenYellowDebugMon sPartyTower[] =
{
    { SPECIES_CHARMELEON, 30 },
    { SPECIES_IVYSAUR, 30 },
    { SPECIES_WARTORTLE, 30 },
};

static const struct GoldenYellowDebugMon sPartySilph[] =
{
    { SPECIES_CHARIZARD, 42 },
    { SPECIES_VENUSAUR, 42 },
    { SPECIES_BLASTOISE, 42 },
};

static const struct GoldenYellowDebugMon sPartyLateRoute22[] =
{
    { SPECIES_CHARIZARD, 55 },
    { SPECIES_VENUSAUR, 55 },
    { SPECIES_BLASTOISE, 55 },
    { SPECIES_SNORLAX, 55 },
};

static const struct GoldenYellowDebugMon sPartyChampion[] =
{
    { SPECIES_CHARIZARD, 65 },
    { SPECIES_VENUSAUR, 65 },
    { SPECIES_BLASTOISE, 65 },
    { SPECIES_SNORLAX, 65 },
    { SPECIES_LAPRAS, 65 },
};

static void GoldenYellowDebug_ResetStoryState(void)
{
    if (PlayerHasFollowerNPC())
        DestroyFollowerNPC();

    ClearFollowerNPCData();
    InitEventData();
    RunScriptImmediately(EventScript_ResetAllMapFlagsFrlg);
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    ClearBag();
    SetMoney(&gSaveBlock1Ptr->money, 3000);
}

static void GoldenYellowDebug_SetBadges(u8 count)
{
    u8 i;

    for (i = 0; i < NUM_BADGES; i++)
    {
        if (i < count)
            FlagSet(gBadgeFlags[i]);
        else
            FlagClear(gBadgeFlags[i]);
    }
}

static void GoldenYellowDebug_SetParty(const struct GoldenYellowDebugMon *party, u8 count)
{
    u8 i;

    for (i = 0; i < count; i++)
        ScriptGiveMon(party[i].species, party[i].level, ITEM_NONE);
}

static void GoldenYellowDebug_CreatePartnerPikachu(const struct GoldenYellowDebugPartnerProfile *profile)
{
    u8 i;

    ScriptGiveMon(SPECIES_PIKACHU_STARTER, profile->level, ITEM_NONE);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][0], profile->moves[i], i);
}

static void GoldenYellowDebug_SetTestParty(const struct GoldenYellowDebugPartnerProfile *partner,
                                            const struct GoldenYellowDebugMon *party,
                                            u8 count)
{
    GoldenYellowDebug_CreatePartnerPikachu(partner);
    GoldenYellowDebug_SetParty(party, count);
}

static void GoldenYellowDebug_ApplyNewGameBaseline(void)
{
    FlagClear(FLAG_GOT_TRAINER_WATCH);
    FlagClear(FLAG_TRAINER_WATCH_TIME_VALID);
    FlagClear(FLAG_PARTNER_PIKACHU_FOLLOWING);
    FlagClear(FLAG_BEAT_RIVAL_IN_OAKS_LAB);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_1F, 0);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_OAK, 0);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 0);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 0);
    VarSet(VAR_YELLOW_RIVAL_EEVEE_STATE, GY_DEBUG_RIVAL_UNSET);
    FlagSet(FLAG_HIDE_PALLET_WILD_PIKACHU);
    FlagSet(FLAG_HIDE_ROUTE1_YELLOW_OAK);
    FlagSet(FLAG_HIDE_OAKS_LAB_YELLOW_PIKACHU);
}

static void GoldenYellowDebug_ApplyThroughTrainerWatch(void)
{
    FlagSet(FLAG_GOT_TRAINER_WATCH);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_1F, 1);

    // Match Mom's Watch setup semantics. Real/emulated RTC is preferred;
    // hardware without RTC receives a deterministic valid fallback.
    if (TrainerWatchCheckClock() == 0)
        TrainerWatchSetManualDateTime(2026, MONTH_JAN, 1, 12, 0);
}

static void GoldenYellowDebug_ApplyRoute1OakReady(void)
{
    GoldenYellowDebug_ApplyThroughTrainerWatch();
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_OAK, 0);
    FlagSet(FLAG_HIDE_OAK_IN_PALLET_TOWN);
    FlagSet(FLAG_HIDE_ROUTE1_YELLOW_OAK);
    FlagSet(FLAG_HIDE_PALLET_WILD_PIKACHU);
}

static void GoldenYellowDebug_ApplyOakLabRivalReady(void)
{
    GoldenYellowDebug_ApplyRoute1OakReady();

    // Persistent results of the accepted Route 1 capture and Lab gift scenes.
    FlagSet(FLAG_VISITED_OAKS_LAB);
    FlagSet(FLAG_SYS_POKEMON_GET);
    FlagSet(FLAG_PALLET_LADY_NOT_BLOCKING_SIGN);
    FlagSet(FLAG_HIDE_PALLET_WILD_PIKACHU);
    FlagSet(FLAG_HIDE_ROUTE1_YELLOW_OAK);
    FlagSet(FLAG_HIDE_OAK_IN_PALLET_TOWN);
    FlagClear(FLAG_HIDE_OAK_IN_HIS_LAB);
    FlagClear(FLAG_HIDE_RIVAL_IN_LAB);
    FlagSet(FLAG_HIDE_OAKS_LAB_YELLOW_PIKACHU);

    // Yellow's Eevee is taken and the captured Pikachu is given to the player;
    // all three FRLG table balls are therefore gone before the rival battle.
    FlagSet(FLAG_HIDE_BULBASAUR_BALL);
    FlagSet(FLAG_HIDE_SQUIRTLE_BALL);
    FlagSet(FLAG_HIDE_CHARMANDER_BALL);

    VarSet(VAR_STARTER_MON, 2);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_OAK, 1);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 3);
    VarSet(VAR_YELLOW_RIVAL_EEVEE_STATE, GY_DEBUG_RIVAL_UNSET);

    GoldenYellowDebug_CreatePartnerPikachu(&sPartnerOakLab);
}

static void GoldenYellowDebug_ApplyOakLabComplete(void)
{
    GoldenYellowDebug_ApplyOakLabRivalReady();
    FlagSet(FLAG_BEAT_RIVAL_IN_OAKS_LAB);
    FlagSet(FLAG_PARTNER_PIKACHU_FOLLOWING);
    FlagSet(FLAG_HIDE_RIVAL_IN_LAB);
    FlagSet(FLAG_HIDE_OAKS_LAB_YELLOW_PIKACHU);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 4);
}

static void GoldenYellowDebug_ApplyPokedexState(void)
{
    FlagSet(FLAG_SYS_POKEDEX_GET);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6);
    VarSet(VAR_MAP_SCENE_VIRIDIAN_CITY_MART, 2);
    VarSet(VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1);
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 1);
    VarSet(VAR_MAP_SCENE_ROUTE22, 1);
}

static void GoldenYellowDebug_SetRivalPath(enum GoldenYellowDebugRivalPath path)
{
    if (path > GY_DEBUG_RIVAL_VAPOREON)
        path = GY_DEBUG_RIVAL_JOLTEON;
    VarSet(VAR_YELLOW_RIVAL_EEVEE_STATE, path);
}

static void GoldenYellowDebug_CompleteRoute22Early(void)
{
    VarSet(VAR_MAP_SCENE_ROUTE22, 2);
    FlagSet(FLAG_HIDE_ROUTE_22_RIVAL);
}

static void GoldenYellowDebug_CompleteCeruleanRival(void)
{
    VarSet(VAR_MAP_SCENE_CERULEAN_CITY_RIVAL, 1);
    FlagSet(FLAG_HIDE_CERULEAN_RIVAL);
}

static void GoldenYellowDebug_CompleteSSAnneRival(void)
{
    VarSet(VAR_MAP_SCENE_S_S_ANNE_2F_CORRIDOR, 1);
    FlagSet(FLAG_HIDE_SS_ANNE_RIVAL);
}

static void GoldenYellowDebug_CompleteTowerRival(void)
{
    VarSet(VAR_MAP_SCENE_POKEMON_TOWER_2F, 1);
    FlagSet(FLAG_HIDE_TOWER_RIVAL);
}

static void GoldenYellowDebug_CompleteSilphRival(void)
{
    VarSet(VAR_MAP_SCENE_SILPH_CO_7F, 1);
    FlagSet(FLAG_HIDE_SILPH_RIVAL);
}

static void GoldenYellowDebug_Warp(const struct GoldenYellowDebugWarp *warp)
{
    SetWarpDestination(MAP_GROUP(warp->map), MAP_NUM(warp->map), WARP_ID_NONE, warp->x, warp->y);
    DoWarp();
    ResetInitialPlayerAvatarState();
}

bool32 GoldenYellowDebug_ApplyCheckpoint(enum GoldenYellowDebugCheckpoint checkpoint,
                                          enum GoldenYellowDebugRivalPath rivalPath)
{
    static const struct GoldenYellowDebugWarp sWarps[GY_DEBUG_CP_COUNT] =
    {
        [GY_DEBUG_CP_BEFORE_TRAINER_WATCH] = { MAP_PALLET_TOWN_PLAYERS_HOUSE_1F, 5, 7 },
        [GY_DEBUG_CP_BEFORE_ROUTE1_OAK] = { MAP_ROUTE1, 12, 40 },
        [GY_DEBUG_CP_OAK_LAB_RIVAL] = { MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6, 9 },
        [GY_DEBUG_CP_ROUTE22_EARLY] = { MAP_ROUTE22, 34, 5 },
        [GY_DEBUG_CP_CERULEAN_RIVAL] = { MAP_CERULEAN_CITY, 23, 7 },
        [GY_DEBUG_CP_SS_ANNE_RIVAL] = { MAP_SSANNE_2F_CORRIDOR, 31, 7 },
        [GY_DEBUG_CP_POKEMON_TOWER_RIVAL] = { MAP_POKEMON_TOWER_2F, 16, 7 },
        [GY_DEBUG_CP_SILPH_RIVAL] = { MAP_SILPH_CO_7F, 2, 3 },
        [GY_DEBUG_CP_ROUTE22_LATE_RIVAL] = { MAP_ROUTE22, 34, 5 },
        [GY_DEBUG_CP_CHAMPION_RIVAL] = { MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM, 6, 19 },
    };

    if (checkpoint >= GY_DEBUG_CP_COUNT)
        return FALSE;

    GoldenYellowDebug_ResetStoryState();
    GoldenYellowDebug_ApplyNewGameBaseline();

    switch (checkpoint)
    {
    case GY_DEBUG_CP_BEFORE_TRAINER_WATCH:
        GoldenYellowDebug_SetBadges(0);
        break;
    case GY_DEBUG_CP_BEFORE_ROUTE1_OAK:
        GoldenYellowDebug_ApplyRoute1OakReady();
        GoldenYellowDebug_SetBadges(0);
        break;
    case GY_DEBUG_CP_OAK_LAB_RIVAL:
        GoldenYellowDebug_ApplyOakLabRivalReady();
        GoldenYellowDebug_SetBadges(0);
        break;
    case GY_DEBUG_CP_ROUTE22_EARLY:
        GoldenYellowDebug_ApplyOakLabComplete();
        GoldenYellowDebug_ApplyPokedexState();
        GoldenYellowDebug_SetRivalPath(GY_DEBUG_RIVAL_FLAREON);
        VarSet(VAR_MAP_SCENE_ROUTE22, 1);
        FlagClear(FLAG_HIDE_ROUTE_22_RIVAL);
        GoldenYellowDebug_SetBadges(0);
        ZeroPlayerPartyMons();
        GoldenYellowDebug_SetTestParty(&sPartnerRoute22, sPartyRoute22, ARRAY_COUNT(sPartyRoute22));
        break;
    case GY_DEBUG_CP_CERULEAN_RIVAL:
        GoldenYellowDebug_ApplyOakLabComplete();
        GoldenYellowDebug_ApplyPokedexState();
        GoldenYellowDebug_CompleteRoute22Early();
        GoldenYellowDebug_SetRivalPath(GY_DEBUG_RIVAL_JOLTEON);
        VarSet(VAR_MAP_SCENE_CERULEAN_CITY_RIVAL, 0);
        FlagClear(FLAG_HIDE_CERULEAN_RIVAL);
        GoldenYellowDebug_SetBadges(1);
        ZeroPlayerPartyMons();
        GoldenYellowDebug_SetTestParty(&sPartnerCerulean, sPartyCerulean, ARRAY_COUNT(sPartyCerulean));
        break;
    case GY_DEBUG_CP_SS_ANNE_RIVAL:
        GoldenYellowDebug_ApplyOakLabComplete();
        GoldenYellowDebug_ApplyPokedexState();
        GoldenYellowDebug_CompleteRoute22Early();
        GoldenYellowDebug_CompleteCeruleanRival();
        GoldenYellowDebug_SetRivalPath(GY_DEBUG_RIVAL_JOLTEON);
        VarSet(VAR_MAP_SCENE_S_S_ANNE_2F_CORRIDOR, 0);
        FlagClear(FLAG_HIDE_SS_ANNE_RIVAL);
        GoldenYellowDebug_SetBadges(2);
        ZeroPlayerPartyMons();
        GoldenYellowDebug_SetTestParty(&sPartnerSSAnne, sPartySSAnne, ARRAY_COUNT(sPartySSAnne));
        break;
    case GY_DEBUG_CP_POKEMON_TOWER_RIVAL:
        GoldenYellowDebug_ApplyOakLabComplete();
        GoldenYellowDebug_ApplyPokedexState();
        GoldenYellowDebug_CompleteRoute22Early();
        GoldenYellowDebug_CompleteCeruleanRival();
        GoldenYellowDebug_CompleteSSAnneRival();
        GoldenYellowDebug_SetRivalPath(rivalPath);
        VarSet(VAR_MAP_SCENE_POKEMON_TOWER_2F, 0);
        FlagClear(FLAG_HIDE_TOWER_RIVAL);
        GoldenYellowDebug_SetBadges(4);
        ZeroPlayerPartyMons();
        GoldenYellowDebug_SetTestParty(&sPartnerTower, sPartyTower, ARRAY_COUNT(sPartyTower));
        break;
    case GY_DEBUG_CP_SILPH_RIVAL:
        GoldenYellowDebug_ApplyOakLabComplete();
        GoldenYellowDebug_ApplyPokedexState();
        GoldenYellowDebug_CompleteRoute22Early();
        GoldenYellowDebug_CompleteCeruleanRival();
        GoldenYellowDebug_CompleteSSAnneRival();
        GoldenYellowDebug_CompleteTowerRival();
        GoldenYellowDebug_SetRivalPath(rivalPath);
        VarSet(VAR_MAP_SCENE_SILPH_CO_7F, 0);
        FlagClear(FLAG_HIDE_SILPH_RIVAL);
        GoldenYellowDebug_SetBadges(4);
        ZeroPlayerPartyMons();
        GoldenYellowDebug_SetTestParty(&sPartnerSilph, sPartySilph, ARRAY_COUNT(sPartySilph));
        break;
    case GY_DEBUG_CP_ROUTE22_LATE_RIVAL:
        GoldenYellowDebug_ApplyOakLabComplete();
        GoldenYellowDebug_ApplyPokedexState();
        GoldenYellowDebug_CompleteRoute22Early();
        GoldenYellowDebug_CompleteCeruleanRival();
        GoldenYellowDebug_CompleteSSAnneRival();
        GoldenYellowDebug_CompleteTowerRival();
        GoldenYellowDebug_CompleteSilphRival();
        GoldenYellowDebug_SetRivalPath(rivalPath);
        VarSet(VAR_MAP_SCENE_ROUTE22, 3);
        FlagClear(FLAG_HIDE_ROUTE_22_RIVAL);
        GoldenYellowDebug_SetBadges(8);
        ZeroPlayerPartyMons();
        GoldenYellowDebug_SetTestParty(&sPartnerLateRoute22, sPartyLateRoute22, ARRAY_COUNT(sPartyLateRoute22));
        break;
    case GY_DEBUG_CP_CHAMPION_RIVAL:
        GoldenYellowDebug_ApplyOakLabComplete();
        GoldenYellowDebug_ApplyPokedexState();
        GoldenYellowDebug_CompleteRoute22Early();
        GoldenYellowDebug_CompleteCeruleanRival();
        GoldenYellowDebug_CompleteSSAnneRival();
        GoldenYellowDebug_CompleteTowerRival();
        GoldenYellowDebug_CompleteSilphRival();
        GoldenYellowDebug_SetRivalPath(rivalPath);
        VarSet(VAR_MAP_SCENE_ROUTE22, 4);
        FlagSet(FLAG_HIDE_ROUTE_22_RIVAL);
        FlagClear(FLAG_SYS_GAME_CLEAR);
        FlagClear(FLAG_IS_CHAMPION);
        GoldenYellowDebug_SetBadges(8);
        ZeroPlayerPartyMons();
        GoldenYellowDebug_SetTestParty(&sPartnerChampion, sPartyChampion, ARRAY_COUNT(sPartyChampion));
        break;
    default:
        return FALSE;
    }

    GoldenYellowDebug_Warp(&sWarps[checkpoint]);
    return TRUE;
}
