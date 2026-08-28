#ifndef GUARD_GOLDEN_YELLOW_DEBUG_H
#define GUARD_GOLDEN_YELLOW_DEBUG_H

#include "global.h"

// Golden Yellow story checkpoints are cumulative developer-state reconstructions.
// They must never replace the corresponding production story scripts.
enum GoldenYellowDebugCheckpoint
{
    GY_DEBUG_CP_BEFORE_TRAINER_WATCH,
    GY_DEBUG_CP_BEFORE_ROUTE1_OAK,
    GY_DEBUG_CP_OAK_LAB_RIVAL,
    GY_DEBUG_CP_ROUTE22_EARLY,
    GY_DEBUG_CP_CERULEAN_RIVAL,
    GY_DEBUG_CP_SS_ANNE_RIVAL,
    GY_DEBUG_CP_POKEMON_TOWER_RIVAL,
    GY_DEBUG_CP_SILPH_RIVAL,
    GY_DEBUG_CP_ROUTE22_LATE_RIVAL,
    GY_DEBUG_CP_CHAMPION_RIVAL,

    // D1 — Early Kanto story checkpoint spine. Appended after the accepted D0
    // IDs so existing debug selections remain numerically stable.
    GY_DEBUG_CP_BEFORE_BROCK,
    GY_DEBUG_CP_MT_MOON_FOSSIL,
    GY_DEBUG_CP_MT_MOON_JESSIE_JAMES,
    GY_DEBUG_CP_BEFORE_MISTY,
    GY_DEBUG_CP_NUGGET_BRIDGE_ROCKET,
    GY_DEBUG_CP_BEFORE_BILL,
    GY_DEBUG_CP_BULBASAUR_GIFT,
    GY_DEBUG_CP_CHARMANDER_GIFT,
    GY_DEBUG_CP_SS_ANNE_CAPTAIN,
    GY_DEBUG_CP_BEFORE_LT_SURGE,
    GY_DEBUG_CP_SQUIRTLE_GIFT,

    // Stage 1B pre-change validation boundaries. Keep these appended so all
    // previously accepted checkpoint IDs remain stable; the debug UI presents
    // them in chronological campaign order independently of numeric ID.
    GY_DEBUG_CP_BEFORE_TEACHY_TV,
    GY_DEBUG_CP_BEFORE_PEWTER_GUIDE,
    GY_DEBUG_CP_BEFORE_RUNNING_SHOES,
    GY_DEBUG_CP_BEFORE_JIGGLYPUFF,

    GY_DEBUG_CP_COUNT,
};

enum GoldenYellowDebugRivalPath
{
    GY_DEBUG_RIVAL_UNSET = 0,
    GY_DEBUG_RIVAL_JOLTEON = 1,
    GY_DEBUG_RIVAL_FLAREON = 2,
    GY_DEBUG_RIVAL_VAPOREON = 3,
};

bool32 GoldenYellowDebug_ApplyCheckpoint(enum GoldenYellowDebugCheckpoint checkpoint,
                                          enum GoldenYellowDebugRivalPath rivalPath);

#endif // GUARD_GOLDEN_YELLOW_DEBUG_H
