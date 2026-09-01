#include "global.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "fldeff.h"
#include "golden_yellow_partner_state.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "constants/metatile_labels.h"

static EWRAM_DATA u8 sEscalatorAnim_TaskId = 0;
static EWRAM_DATA struct ObjectEvent *sPartnerFollowerToRestore = NULL;

static void SetEscalatorMetatile(u8 taskId, const s16 *metatileIds, u16 metatileMasks);
static void Task_DrawEscalator(u8 taskId);

#define ESCALATOR_STAGES     3
#define LAST_ESCALATOR_STAGE (ESCALATOR_STAGES - 1)

static const s16 sEscalatorMetatiles_1F_0[ESCALATOR_STAGES] = {
    METATILE_PokemonCenter_Escalator1F_Tile0_Frame2,
    METATILE_PokemonCenter_Escalator1F_Tile0_Frame1,
    METATILE_PokemonCenter_Escalator1F_Tile0_Frame0
};

static const s16 sEscalatorMetatiles_1F_1[ESCALATOR_STAGES] = {
    METATILE_PokemonCenter_Escalator1F_Tile1_Frame2,
    METATILE_PokemonCenter_Escalator1F_Tile1_Frame1,
    METATILE_PokemonCenter_Escalator1F_Tile1_Frame0
};

static const s16 sEscalatorMetatiles_1F_2[ESCALATOR_STAGES] = {
    METATILE_PokemonCenter_Escalator1F_Tile2_Frame2,
    METATILE_PokemonCenter_Escalator1F_Tile2_Frame1,
    METATILE_PokemonCenter_Escalator1F_Tile2_Frame0
};

static const s16 sEscalatorMetatiles_1F_3[ESCALATOR_STAGES] = {
    METATILE_PokemonCenter_Escalator1F_Tile3_Frame2,
    METATILE_PokemonCenter_Escalator1F_Tile3_Frame1,
    METATILE_PokemonCenter_Escalator1F_Tile3_Frame0
};

static const s16 sEscalatorMetatiles_2F_0[ESCALATOR_STAGES] = {
    METATILE_PokemonCenter_Escalator2F_Tile0_Frame0,
    METATILE_PokemonCenter_Escalator2F_Tile0_Frame1,
    METATILE_PokemonCenter_Escalator2F_Tile0_Frame2
};

static const s16 sEscalatorMetatiles_2F_1[ESCALATOR_STAGES] = {
    METATILE_PokemonCenter_Escalator2F_Tile1_Frame0,
    METATILE_PokemonCenter_Escalator2F_Tile1_Frame1,
    METATILE_PokemonCenter_Escalator2F_Tile1_Frame2
};

static const s16 sEscalatorMetatiles_2F_2[ESCALATOR_STAGES] = {
    METATILE_PokemonCenter_Escalator2F_Tile2_Frame0,
    METATILE_PokemonCenter_Escalator2F_Tile2_Frame1,
    METATILE_PokemonCenter_Escalator2F_Tile2_Frame2
};

static const s16 sEscalatorMetatilesFrlg_BottomNextRail[ESCALATOR_STAGES] = 
{
    METATILE_PokemonCenterFrlg_Escalator_BottomNextRail_Transition2, 
    METATILE_PokemonCenterFrlg_Escalator_BottomNextRail_Transition1, 
    METATILE_PokemonCenterFrlg_Escalator_BottomNextRail_Normal
};

static const s16 sEscalatorMetatilesFrlg_BottomRail[ESCALATOR_STAGES] = 
{
    METATILE_PokemonCenterFrlg_Escalator_BottomRail_Transition2, 
    METATILE_PokemonCenterFrlg_Escalator_BottomRail_Transition1, 
    METATILE_PokemonCenterFrlg_Escalator_BottomRail_Normal
};

static const s16 sEscalatorMetatilesFrlg_BottomNext[ESCALATOR_STAGES] = 
{
    METATILE_PokemonCenterFrlg_Escalator_BottomNext_Transition2, 
    METATILE_PokemonCenterFrlg_Escalator_BottomNext_Transition1, 
    METATILE_PokemonCenterFrlg_Escalator_BottomNext_Normal
};

static const s16 sEscalatorMetatilesFrlg_Bottom[ESCALATOR_STAGES] = 
{
    METATILE_PokemonCenterFrlg_Escalator_Bottom_Transition2, 
    METATILE_PokemonCenterFrlg_Escalator_Bottom_Transition1, 
    METATILE_PokemonCenterFrlg_Escalator_Bottom_Normal
};

static const s16 sEscalatorMetatilesFrlg_TopNext[ESCALATOR_STAGES] = 
{
    METATILE_PokemonCenterFrlg_Escalator_TopNext_Normal, 
    METATILE_PokemonCenterFrlg_Escalator_TopNext_Transition1, 
    METATILE_PokemonCenterFrlg_Escalator_TopNext_Transition2
};

static const s16 sEscalatorMetatilesFrlg_Top[ESCALATOR_STAGES] = 
{
    METATILE_PokemonCenterFrlg_Escalator_Top_Normal, 
    METATILE_PokemonCenterFrlg_Escalator_Top_Transition1, 
    METATILE_PokemonCenterFrlg_Escalator_Top_Transition2
};

static const s16 sEscalatorMetatilesFrlg_TopNextRail[ESCALATOR_STAGES] = 
{
    METATILE_PokemonCenterFrlg_Escalator_TopNextRail_Normal, 
    METATILE_PokemonCenterFrlg_Escalator_TopNextRail_Transition1, 
    METATILE_PokemonCenterFrlg_Escalator_TopNextRail_Transition2
};

#define tState            data[0]
#define tTransitionStage  data[1]
#define tGoingUp          data[2]
#define tDrawingEscalator data[3]
#define tPlayerX          data[4]
#define tPlayerY          data[5]

static void SetEscalatorMetatile(u8 taskId, const s16 *metatileIds, u16 metatileMasks)
{
    s16 x = gTasks[taskId].tPlayerX - 1;
    s16 y = gTasks[taskId].tPlayerY - 1;
    s16 transitionStage = gTasks[taskId].tTransitionStage;
    s16 i;
    s16 j;

    // Check all the escalator sections and only progress the selected one to the next stage
    if (!gTasks[taskId].tGoingUp)
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                s16 metatileId = MapGridGetMetatileIdAt(x + j, y + i);

                if (metatileIds[transitionStage] == metatileId)
                {
                    if (transitionStage != LAST_ESCALATOR_STAGE)
                        MapGridSetMetatileIdAt(x + j, y + i, metatileMasks | metatileIds[transitionStage + 1]);
                    else
                        MapGridSetMetatileIdAt(x + j, y + i, metatileMasks | metatileIds[0]);
                }
            }
        }
    }
    else
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                s16 metatileId = MapGridGetMetatileIdAt(x + j, y + i);

                if (metatileIds[LAST_ESCALATOR_STAGE - transitionStage] == metatileId)
                {
                    if (transitionStage != LAST_ESCALATOR_STAGE)
                        MapGridSetMetatileIdAt(x + j, y + i, metatileMasks | metatileIds[1 - transitionStage]);
                    else
                        MapGridSetMetatileIdAt(x + j, y + i, metatileMasks | metatileIds[LAST_ESCALATOR_STAGE]);
                }
            }
        }
    }
}

static void Task_DrawEscalator(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    // CreateEscalatorTask draws state 0 synchronously. On an outbound ordinary
    // escalator, StartEscalator has temporarily marked the canonical Partner as
    // non-recallable so HideFollowerForFieldEffect immediately after this call
    // takes its existing invisible-object early return. Restore the object on
    // the first asynchronous escalator tick, before normal field rendering.
    if (sPartnerFollowerToRestore != NULL && tState != 0)
    {
        if (sPartnerFollowerToRestore->active)
        {
            sPartnerFollowerToRestore->invisible = FALSE;
            gSprites[sPartnerFollowerToRestore->spriteId].invisible = FALSE;
        }
        sPartnerFollowerToRestore = NULL;
    }

    tDrawingEscalator = TRUE;

    // Set tile for each section of the escalator in sequence for current transition stage
    switch (tState)
    {
    case 0:
        SetEscalatorMetatile(taskId, gMapHeader.mapLayout->isFrlg ? sEscalatorMetatilesFrlg_BottomNextRail : sEscalatorMetatiles_1F_0, 0);
        break;
    case 1:
        SetEscalatorMetatile(taskId, gMapHeader.mapLayout->isFrlg ? sEscalatorMetatilesFrlg_BottomRail : sEscalatorMetatiles_1F_1, 0);
        break;
    case 2:
        SetEscalatorMetatile(taskId, gMapHeader.mapLayout->isFrlg ? sEscalatorMetatilesFrlg_BottomNext : sEscalatorMetatiles_1F_2, MAPGRID_IMPASSABLE);
        break;
    case 3:
        SetEscalatorMetatile(taskId, gMapHeader.mapLayout->isFrlg ? sEscalatorMetatilesFrlg_Bottom : sEscalatorMetatiles_1F_3, 0);
        break;
    case 4:
        SetEscalatorMetatile(taskId, gMapHeader.mapLayout->isFrlg ? sEscalatorMetatilesFrlg_TopNext : sEscalatorMetatiles_2F_0, MAPGRID_IMPASSABLE);
        break;
    case 5:
        SetEscalatorMetatile(taskId, gMapHeader.mapLayout->isFrlg ? sEscalatorMetatilesFrlg_Top : sEscalatorMetatiles_2F_1, 0);
        break;
    case 6:
        SetEscalatorMetatile(taskId, gMapHeader.mapLayout->isFrlg ? sEscalatorMetatilesFrlg_TopNextRail : sEscalatorMetatiles_2F_2, 0);
        break;
    }

    tState = (tState + 1) & 7;

    // If all metatiles of the escalator have been set, draw map and progress to next stage
    if (tState == 0)
    {
        DrawWholeMapView();
        tTransitionStage = (tTransitionStage + 1) % ESCALATOR_STAGES;
        tDrawingEscalator = FALSE;
    }
}

static u8 CreateEscalatorTask(bool16 goingUp)
{
    u8 taskId = CreateTask(Task_DrawEscalator, 0);
    s16 *data = gTasks[taskId].data;

    PlayerGetDestCoords(&tPlayerX, &tPlayerY);
    tState = 0;
    tTransitionStage = 0;
    tGoingUp = goingUp;
    Task_DrawEscalator(taskId);
    return taskId;
}

void StartEscalator(bool8 goingUp)
{
    struct ObjectEvent *follower = GetFollowerObject();

    sPartnerFollowerToRestore = NULL;

    // The ordinary escalator warp calls StartEscalator immediately before the
    // generic field-effect recall. Yellow's canonical Partner stays outside its
    // Poké Ball for this traversal. The fade guard keeps the destination-side
    // escalator animation out of this outbound-only recall bypass.
    if (!gPaletteFade.active
     && GoldenYellow_IsCanonicalPartnerPikachuFollower(follower)
     && !follower->invisible)
    {
        follower->invisible = TRUE;
        gSprites[follower->spriteId].invisible = FALSE;
        sPartnerFollowerToRestore = follower;
    }

    sEscalatorAnim_TaskId = CreateEscalatorTask(goingUp);
}

void StopEscalator(void)
{
    if (sPartnerFollowerToRestore != NULL)
    {
        if (sPartnerFollowerToRestore->active)
        {
            sPartnerFollowerToRestore->invisible = FALSE;
            gSprites[sPartnerFollowerToRestore->spriteId].invisible = FALSE;
        }
        sPartnerFollowerToRestore = NULL;
    }

    DestroyTask(sEscalatorAnim_TaskId);
}

bool8 IsEscalatorMoving(void)
{
    if (gTasks[sEscalatorAnim_TaskId].tDrawingEscalator == FALSE
     && gTasks[sEscalatorAnim_TaskId].tTransitionStage == LAST_ESCALATOR_STAGE)
        return FALSE;
    else
        return TRUE;
}

#undef tState
#undef tTransitionStage
#undef tGoingUp
#undef tDrawingEscalator
#undef tPlayerX
#undef tPlayerY