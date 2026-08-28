#include "global.h"
#include "event_object_movement.h"
#include "script.h"
#include "sprite.h"

static struct ObjectEvent *GoldenYellowPartner_GetActiveFollower(void)
{
    struct ObjectEvent *follower;

    UpdateFollowingPokemon();
    follower = GetFollowerObject();
    if (follower == NULL || !follower->active)
        return NULL;

    return follower;
}

void GoldenYellowPartner_HoldAtBillDoor(void)
{
    struct ObjectEvent *follower;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    follower = GoldenYellowPartner_GetActiveFollower();
    if (follower == NULL)
        return;

    ObjectEventClearHeldMovementIfActive(follower);
    UnfreezeObjectEvent(follower);
    SetTrainerMovementType(follower, MOVEMENT_TYPE_NONE);
    ObjectEventTurn(follower, DIR_NORTH);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}

void GoldenYellowPartner_ResumeFollowing(void)
{
    struct ObjectEvent *follower;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    follower = GoldenYellowPartner_GetActiveFollower();
    if (follower == NULL)
        return;

    ObjectEventClearHeldMovementIfActive(follower);
    SetTrainerMovementType(follower, MOVEMENT_TYPE_FOLLOW_PLAYER);
    UnfreezeObjectEvent(follower);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}
