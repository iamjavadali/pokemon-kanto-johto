#include "global.h"
#include "event_object_movement.h"
#include "script.h"
#include "sprite.h"

static struct ObjectEvent *GoldenYellowPartner_GetActiveFollower(void)
{
    struct ObjectEvent *follower = GetFollowerObject();

    if (follower == NULL || !follower->active)
        return NULL;

    return follower;
}

void GoldenYellowPartner_HoldAtBillDoor(void)
{
    struct ObjectEvent *follower;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    // The Before Bill checkpoint can load directly on the cottage approach
    // before the normal follower object has been instantiated. It is safe to
    // create/update the follower here because this helper is only used when
    // taking ownership of Partner Pikachu for the doorway handoff.
    UpdateFollowingPokemon();
    follower = GoldenYellowPartner_GetActiveFollower();
    if (follower == NULL)
        return;

    ObjectEventClearHeldMovementIfActive(follower);
    follower->singleMovementActive = FALSE;
    follower->heldMovementActive = FALSE;
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

    // Never call UpdateFollowingPokemon here. These Route 25 restore triggers
    // can also fire immediately after a normal Sea Cottage warp, where the
    // follower engine already owns Pikachu's door/reappearance lifecycle.
    follower = GoldenYellowPartner_GetActiveFollower();
    if (follower == NULL)
        return;

    // Only undo the temporary doorway hold that GoldenYellowPartner_HoldAtBillDoor
    // applied. If Pikachu is already following normally, leave the runtime
    // follower state completely untouched.
    if (follower->movementType != MOVEMENT_TYPE_NONE)
        return;

    ObjectEventClearHeldMovementIfActive(follower);
    follower->singleMovementActive = FALSE;
    follower->heldMovementActive = FALSE;
    SetTrainerMovementType(follower, MOVEMENT_TYPE_FOLLOW_PLAYER);
    UnfreezeObjectEvent(follower);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}
