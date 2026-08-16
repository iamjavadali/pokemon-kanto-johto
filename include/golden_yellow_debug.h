#ifndef GUARD_GOLDEN_YELLOW_DEBUG_H
#define GUARD_GOLDEN_YELLOW_DEBUG_H

#include "global.h"

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
