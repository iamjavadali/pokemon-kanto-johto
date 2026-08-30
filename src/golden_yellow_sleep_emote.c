#include "global.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "field_emote_icons.h"
#include "field_player_avatar.h"
#include "script.h"
#include "task.h"
#include "constants/field_effects.h"

static void Task_WaitForGoldenYellowSleepEmote(u8 taskId);
static bool8 GoldenYellow_WaitForPlayerFaceFollower(void);

void GoldenYellow_ShowFollowerSleepEmote(void)
{
    struct ObjectEvent *follower = GetFollowerObject();
    u8 taskId;

    if (follower == NULL || !follower->active)
    {
        ScriptContext_Enable();
        return;
    }

    taskId = CreateTask(Task_WaitForGoldenYellowSleepEmote, 0x52);
    if (taskId == TASK_NONE)
    {
        ScriptContext_Enable();
        return;
    }

    if (!FieldEmote_StartOnObjectEvent(follower, FIELD_EMOTE_SLEEP))
    {
        DestroyTask(taskId);
        ScriptContext_Enable();
    }
}

void GoldenYellow_FacePlayerTowardFollower(struct ScriptContext *ctx)
{
    struct ObjectEvent *follower = GetFollowerObject();
    struct ObjectEvent *player;
    s16 deltaX;
    s16 deltaY;
    enum Direction direction;

    if (follower == NULL || !follower->active)
        return;

    player = &gObjectEvents[gPlayerAvatar.objectEventId];
    deltaX = follower->currentCoords.x - player->currentCoords.x;
    deltaY = follower->currentCoords.y - player->currentCoords.y;

    if (deltaX > 0)
        direction = DIR_EAST;
    else if (deltaX < 0)
        direction = DIR_WEST;
    else if (deltaY > 0)
        direction = DIR_SOUTH;
    else if (deltaY < 0)
        direction = DIR_NORTH;
    else
        return;

    ObjectEventClearHeldMovementIfActive(player);
    if (ObjectEventSetHeldMovement(player, GetFaceDirectionMovementAction(direction)))
        return;

    SetupNativeScript(ctx, GoldenYellow_WaitForPlayerFaceFollower);
}

static void Task_WaitForGoldenYellowSleepEmote(u8 taskId)
{
    if (!FieldEffectActiveListContains(FLDEFF_SHARED_EMOTE_ICON))
    {
        DestroyTask(taskId);
        ScriptContext_Enable();
    }
}

static bool8 GoldenYellow_WaitForPlayerFaceFollower(void)
{
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];

    return ObjectEventClearHeldMovementIfFinished(player) != 0;
}
