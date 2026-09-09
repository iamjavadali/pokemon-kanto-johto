#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "golden_yellow_partner_fan_club.h"
#include "golden_yellow_partner_reaction.h"
#include "golden_yellow_partner_state.h"
#include "pokemon.h"
#include "script.h"
#include "script_movement.h"
#include "sound.h"
#include "sprite.h"
#include "constants/battle.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/maps.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/vars.h"

#define VAR_GY_BILL_PARTNER_RELEASED VAR_TEMP_2
#define BILL_PARTNER_APPROACH_MOVEMENT_CAPACITY 32
#define ROUTE24_SCENE_ROUTE_CAPACITY 32
#define ROUTE24_SCENE_NODE_CAPACITY 128

enum GoldenYellowPartnerInteractionRoute
{
    GY_PARTNER_INTERACTION_NONE,

    // P7E priority tier 1: authored story ownership.
    GY_PARTNER_INTERACTION_STORY_BILL,
    GY_PARTNER_INTERACTION_STORY_FAN_CLUB,
    GY_PARTNER_INTERACTION_STORY_PEWTER_WAKE,

    // P7E priority tier 2: actual battle-status reactions.
    GY_PARTNER_INTERACTION_STATUS,

    // P7E priority tier 3: authored area reaction.
    GY_PARTNER_INTERACTION_AREA_TOWER,

    // P7E priority tiers 4 and 5: P6 one-shot, then P5 mood fallback.
    GY_PARTNER_INTERACTION_ONE_SHOT,
    GY_PARTNER_INTERACTION_MOOD,
};

static EWRAM_DATA u8 sBillPartnerApproachMovement[BILL_PARTNER_APPROACH_MOVEMENT_CAPACITY];
static u8 sBillPartnerApproachLocalId;

static bool8 GoldenYellow_WaitForPartnerPikachuFieldInteraction(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_ClearPartnerPikachuReactionObject();
    return TRUE;
}

static bool8 GoldenYellow_WaitForPewterPartnerWake(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_CompletePewterPartnerWakeOnFollower();
    return TRUE;
}

static bool8 GoldenYellow_WaitForFanClubPartnerRejoin(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_CompleteFanClubPartnerOnFollower();
    return TRUE;
}

static bool32 GoldenYellow_IsInBillSeaCottage(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROUTE25_SEA_COTTAGE)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROUTE25_SEA_COTTAGE);
}

static struct ObjectEvent *GoldenYellow_FindBillPartnerSceneObject(void)
{
    struct ObjectEvent *follower = GetFollowerObject();

    if (!GoldenYellow_IsInBillSeaCottage()
     || follower == NULL
     || !follower->active
     || follower->mapGroup != gSaveBlock1Ptr->location.mapGroup
     || follower->mapNum != gSaveBlock1Ptr->location.mapNum)
        return NULL;

    return follower;
}

static void GoldenYellow_ParkBillPartnerFollower(struct ObjectEvent *follower)
{
    if (follower == NULL || !follower->active)
        return;

    // Match the accepted P7A/P7C ownership handoff: preserve the real follower
    // object, but fully clear any in-flight FOLLOW_PLAYER movement before the
    // authored Bill scene takes control of its position.
    ClearObjectEventMovement(follower, &gSprites[follower->spriteId]);
    UnfreezeObjectEvent(follower);
    SetTrainerMovementType(follower, MOVEMENT_TYPE_NONE);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}

static void GoldenYellow_NormalizeBillPartnerFollower(struct ObjectEvent *follower)
{
    if (follower == NULL || !follower->active)
        return;

    // The same object that was parked for Bill resumes normal following. This
    // avoids the visible remove/recreate handoff that could make Pikachu vanish
    // until the player moved after the post-restoration A-button reaction.
    ClearObjectEventMovement(follower, &gSprites[follower->spriteId]);
    UnfreezeObjectEvent(follower);
    SetTrainerMovementType(follower, MOVEMENT_TYPE_FOLLOW_PLAYER);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}

static struct ObjectEvent *GoldenYellow_FindTransformedBillObject(void)
{
    u32 i;

    if (!GoldenYellow_IsInBillSeaCottage())
        return NULL;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *object = &gObjectEvents[i];

        if (!object->active
         || object->graphicsId != OBJ_EVENT_GFX_CLEFAIRY
         || object->mapGroup != gSaveBlock1Ptr->location.mapGroup
         || object->mapNum != gSaveBlock1Ptr->location.mapNum)
            continue;

        return object;
    }

    return NULL;
}

static bool32 GoldenYellow_AppendBillPartnerApproachMovement(u8 *count, u8 movement)
{
    if (*count >= BILL_PARTNER_APPROACH_MOVEMENT_CAPACITY - 1)
        return FALSE;

    sBillPartnerApproachMovement[(*count)++] = movement;
    return TRUE;
}

static bool32 GoldenYellow_BuildBillPartnerApproachMovement(const struct ObjectEvent *partnerObject,
                                                             const struct ObjectEvent *billObject)
{
    s16 x;
    s16 y;
    s16 targetX;
    s16 targetY;
    u8 count = 0;

    if (partnerObject == NULL || billObject == NULL)
        return FALSE;

    x = partnerObject->currentCoords.x;
    y = partnerObject->currentCoords.y;
    targetX = billObject->currentCoords.x;
    targetY = billObject->currentCoords.y + 1;

    // Match the accepted Fan Club attention beat: standard SE_PIN, visible !,
    // then a full 48-frame pause before Partner begins the authored run.
    if (!GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_EMOTE_EXCLAMATION_MARK)
     || !GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_DELAY_16)
     || !GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_DELAY_16)
     || !GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_DELAY_16))
        return FALSE;

    while (x < targetX)
    {
        if (!GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_RIGHT))
            return FALSE;
        x++;
    }
    while (x > targetX)
    {
        if (!GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_LEFT))
            return FALSE;
        x--;
    }
    while (y < targetY)
    {
        if (!GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_DOWN))
            return FALSE;
        y++;
    }
    while (y > targetY)
    {
        if (!GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_UP))
            return FALSE;
        y--;
    }

    if (!GoldenYellow_AppendBillPartnerApproachMovement(&count, MOVEMENT_ACTION_FACE_UP))
        return FALSE;

    sBillPartnerApproachMovement[count] = MOVEMENT_ACTION_STEP_END;
    return TRUE;
}

static bool8 GoldenYellow_WaitForBillPartnerEntryApproach(void)
{
    struct ObjectEvent *partnerObject;

    if (!GoldenYellow_IsInBillSeaCottage() || sBillPartnerApproachLocalId == 0)
        return TRUE;

    if (!ScriptMovement_IsObjectMovementFinished(sBillPartnerApproachLocalId,
                                                  gSaveBlock1Ptr->location.mapNum,
                                                  gSaveBlock1Ptr->location.mapGroup))
        return FALSE;

    partnerObject = GoldenYellow_FindBillPartnerSceneObject();
    if (partnerObject != NULL)
    {
        ObjectEventClearHeldMovementIfFinished(partnerObject);
        UnfreezeObjectEvent(partnerObject);
    }

    sBillPartnerApproachLocalId = 0;
    return TRUE;
}

void GoldenYellow_FaceBillPartnerSceneObject(struct ScriptContext *ctx)
{
    struct ObjectEvent *partnerObject = GoldenYellow_FindBillPartnerSceneObject();
    enum Direction direction = gSpecialVar_0x8004;

    (void)ctx;

    if (partnerObject == NULL
     || (direction != DIR_NORTH && direction != DIR_SOUTH))
        return;

    // The parked follower can retain the reaction director's last held facing.
    // Clear that ownership and turn directly so the authored Bill scene pose
    // cannot be swallowed by a follower or ScriptMovement callback.
    ObjectEventClearHeldMovementIfActive(partnerObject);
    UnfreezeObjectEvent(partnerObject);
    ObjectEventTurn(partnerObject, direction);
}


static bool32 GoldenYellow_IsInCeruleanHouse3(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_CERULEAN_CITY_HOUSE3)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_CERULEAN_CITY_HOUSE3);
}

static void GoldenYellow_FacePartnerTowardPlayer(void)
{
    struct ObjectEvent *partnerObject = GetFollowerObject();
    struct ObjectEvent *playerObject = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 deltaX;
    s16 deltaY;
    enum Direction direction;

    if (partnerObject == NULL || !partnerObject->active)
        return;

    deltaX = playerObject->currentCoords.x - partnerObject->currentCoords.x;
    deltaY = playerObject->currentCoords.y - partnerObject->currentCoords.y;

    if ((deltaX < 0 ? -deltaX : deltaX) > (deltaY < 0 ? -deltaY : deltaY))
        direction = deltaX < 0 ? DIR_WEST : DIR_EAST;
    else if (deltaY != 0)
        direction = deltaY < 0 ? DIR_NORTH : DIR_SOUTH;
    else
        return;

    ObjectEventClearHeldMovementIfActive(partnerObject);
    UnfreezeObjectEvent(partnerObject);
    ObjectEventTurn(partnerObject, direction);
}

static bool8 GoldenYellow_WaitForMelanieBulbasaurPartnerReaction(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_ClearPartnerPikachuReactionObject();
    GoldenYellow_FacePartnerTowardPlayer();
    return TRUE;
}

void GoldenYellow_StartMelanieBulbasaurPartnerReaction(struct ScriptContext *ctx)
{
    struct ObjectEvent *follower = GetFollowerObject();

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    GoldenYellow_ClearPartnerPikachuReactionObject();

    if (!GoldenYellow_IsInCeruleanHouse3()
     || !GoldenYellow_IsCanonicalPartnerPikachuFollower(follower))
        return;

    GoldenYellow_FacePartnerTowardPlayer();

    if (!GoldenYellow_StartPartnerPikachuReaction(GY_PARTNER_REACTION_CAPTURE_SUCCESS))
        return;

    SetupNativeScript(ctx, GoldenYellow_WaitForMelanieBulbasaurPartnerReaction);
    ctx->waitAfterCallNative = TRUE;
}

static bool32 GoldenYellow_IsOnRoute24(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROUTE24)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROUTE24);
}

enum GoldenYellowRoute24SceneRoute
{
    GY_ROUTE24_ROUTE_PARTNER_WEAK = 1,
    GY_ROUTE24_ROUTE_PARTNER_HEALED,
    GY_ROUTE24_ROUTE_DAMIAN_APPROACH,
    GY_ROUTE24_ROUTE_DAMIAN_CENTER,
    GY_ROUTE24_ROUTE_PARTNER_REJOIN,
    GY_ROUTE24_ROUTE_PARTNER_CANCEL_REJOIN,
};

enum GoldenYellowRoute24DamianLane
{
    GY_ROUTE24_DAMIAN_LANE_RIGHT,
    GY_ROUTE24_DAMIAN_LANE_SOUTH,
};

enum GoldenYellowRoute24PlayerSide
{
    GY_ROUTE24_PLAYER_SIDE_SOUTH,
    GY_ROUTE24_PLAYER_SIDE_RIGHT,
    GY_ROUTE24_PLAYER_SIDE_LEFT,
};

struct GoldenYellowRoute24RouteNode
{
    s16 x;
    s16 y;
    s16 parent;
    enum Direction direction;
};

static EWRAM_DATA u8 sRoute24SceneMovement[ROUTE24_SCENE_ROUTE_CAPACITY];
static EWRAM_DATA struct GoldenYellowRoute24RouteNode sRoute24SceneNodes[ROUTE24_SCENE_NODE_CAPACITY];
static u8 sRoute24SceneLocalId;
static bool8 sRoute24SceneHasForbiddenTile;
static s16 sRoute24SceneForbiddenX;
static s16 sRoute24SceneForbiddenY;

static bool32 GoldenYellow_IsCanonicalRoute24Partner(struct ObjectEvent *follower)
{
    return GoldenYellow_IsOnRoute24()
        && GoldenYellow_IsCanonicalPartnerPikachuFollower(follower);
}

static struct ObjectEvent *GoldenYellow_FindRoute24Charmander(void)
{
    u32 i;

    if (!GoldenYellow_IsOnRoute24())
        return NULL;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *object = &gObjectEvents[i];

        if (object->active
         && object->mapGroup == gSaveBlock1Ptr->location.mapGroup
         && object->mapNum == gSaveBlock1Ptr->location.mapNum
         && object->graphicsId == OBJ_EVENT_GFX_SPECIES(CHARMANDER))
            return object;
    }

    return NULL;
}

static struct ObjectEvent *GoldenYellow_FindRoute24ObjectByLocalId(u8 localId)
{
    u8 objectEventId;

    if (!GoldenYellow_IsOnRoute24()
     || TryGetObjectEventIdByLocalIdAndMap(localId,
                                           gSaveBlock1Ptr->location.mapNum,
                                           gSaveBlock1Ptr->location.mapGroup,
                                           &objectEventId))
        return NULL;

    return &gObjectEvents[objectEventId];
}

static bool32 GoldenYellow_Route24TileHasObject(const struct ObjectEvent *actor, s16 x, s16 y)
{
    const struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    u32 i;

    // The player object is not guaranteed to share the same map identity fields
    // used by regular NPC objects. Reserve its live tile explicitly so a staged
    // Partner route can never cross through the player.
    if (player != actor
     && player->active
     && player->currentCoords.x == x
     && player->currentCoords.y == y)
        return TRUE;

    if (sRoute24SceneHasForbiddenTile
     && sRoute24SceneForbiddenX == x
     && sRoute24SceneForbiddenY == y)
        return TRUE;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        const struct ObjectEvent *object = &gObjectEvents[i];

        if (!object->active || object == actor)
            continue;
        // The scripted camera is an invisible focus anchor, not a physical
        // actor. On the right trigger its panned position overlaps Pikachu's
        // authored approach lane, so it must not block scene pathfinding.
        if (object->localId == LOCALID_CAMERA)
            continue;
        if (object->mapGroup != gSaveBlock1Ptr->location.mapGroup
         || object->mapNum != gSaveBlock1Ptr->location.mapNum)
            continue;
        // Every scene route is planned while all actors are locked and prior
        // movement has completed. Only the live tile remains occupied; using
        // previousCoords here would incorrectly keep Damian's vacated lane
        // blocked during Pikachu's post-adoption rejoin.
        if (object->currentCoords.x == x && object->currentCoords.y == y)
            return TRUE;
    }

    return FALSE;
}

static bool32 GoldenYellow_Route24CanStep(const struct ObjectEvent *actor,
                                           s16 x,
                                           s16 y,
                                           enum Direction direction,
                                           s16 nextX,
                                           s16 nextY)
{
    struct ObjectEvent probe = *actor;

    if (!AreCoordsInsidePlayerMap(nextX, nextY)
     || MapGridGetCollisionAt(nextX, nextY)
     || GetMapBorderIdAt(nextX, nextY) == CONNECTION_INVALID
     || GoldenYellow_Route24TileHasObject(actor, nextX, nextY))
        return FALSE;

    probe.currentCoords.x = x;
    probe.currentCoords.y = y;
    probe.currentElevation = MapGridGetElevationAt(x, y);
    probe.currentMetatileBehavior = MapGridGetMetatileBehaviorAt(x, y);
    probe.range.rangeX = 0;
    probe.range.rangeY = 0;

    if (IsMetatileDirectionallyImpassable(&probe, nextX, nextY, direction)
     || IsElevationMismatchAt(probe.currentElevation, nextX, nextY))
        return FALSE;

    return TRUE;
}

static bool32 GoldenYellow_Route24AppendMovement(u8 *count, u8 movement)
{
    if (*count >= ROUTE24_SCENE_ROUTE_CAPACITY - 1)
        return FALSE;

    sRoute24SceneMovement[(*count)++] = movement;
    return TRUE;
}

static bool32 GoldenYellow_Route24AppendValidatedStep(const struct ObjectEvent *actor,
                                                       s16 *x,
                                                       s16 *y,
                                                       enum Direction direction,
                                                       u8 *count)
{
    s16 nextX = *x;
    s16 nextY = *y;

    MoveCoords(direction, &nextX, &nextY);
    if (!GoldenYellow_Route24CanStep(actor, *x, *y, direction, nextX, nextY)
     || !GoldenYellow_Route24AppendMovement(count, GetWalkNormalMovementAction(direction)))
        return FALSE;

    *x = nextX;
    *y = nextY;
    return TRUE;
}

static bool32 GoldenYellow_Route24NodeWasVisited(s16 x, s16 y, u16 nodeCount)
{
    u16 i;

    for (i = 0; i < nodeCount; i++)
    {
        if (sRoute24SceneNodes[i].x == x && sRoute24SceneNodes[i].y == y)
            return TRUE;
    }

    return FALSE;
}

static bool32 GoldenYellow_Route24AppendPathSegment(const struct ObjectEvent *actor,
                                                     s16 *startX,
                                                     s16 *startY,
                                                     s16 targetX,
                                                     s16 targetY,
                                                     const enum Direction *directionOrder,
                                                     u8 *movementCount)
{
    u8 reversePath[ROUTE24_SCENE_ROUTE_CAPACITY];
    u16 head = 0;
    u16 nodeCount = 1;
    s16 destinationNode = -1;
    s16 minX = (*startX < targetX ? *startX : targetX) - 4;
    s16 maxX = (*startX > targetX ? *startX : targetX) + 4;
    s16 minY = (*startY < targetY ? *startY : targetY) - 4;
    s16 maxY = (*startY > targetY ? *startY : targetY) + 4;
    u8 reverseCount = 0;
    u16 i;

    sRoute24SceneNodes[0].x = *startX;
    sRoute24SceneNodes[0].y = *startY;
    sRoute24SceneNodes[0].parent = -1;
    sRoute24SceneNodes[0].direction = DIR_NONE;

    while (head < nodeCount)
    {
        struct GoldenYellowRoute24RouteNode *node = &sRoute24SceneNodes[head];

        if (node->x == targetX && node->y == targetY)
        {
            destinationNode = head;
            break;
        }

        for (i = 0; i < 4; i++)
        {
            enum Direction direction = directionOrder[i];
            s16 nextX = node->x;
            s16 nextY = node->y;

            MoveCoords(direction, &nextX, &nextY);
            if (nextX < minX || nextX > maxX || nextY < minY || nextY > maxY)
                continue;
            if (GoldenYellow_Route24NodeWasVisited(nextX, nextY, nodeCount))
                continue;
            if (!GoldenYellow_Route24CanStep(actor, node->x, node->y, direction, nextX, nextY))
                continue;
            if (nodeCount >= ROUTE24_SCENE_NODE_CAPACITY)
                return FALSE;

            sRoute24SceneNodes[nodeCount].x = nextX;
            sRoute24SceneNodes[nodeCount].y = nextY;
            sRoute24SceneNodes[nodeCount].parent = head;
            sRoute24SceneNodes[nodeCount].direction = direction;
            nodeCount++;
        }

        head++;
    }

    if (destinationNode < 0)
        return FALSE;

    while (sRoute24SceneNodes[destinationNode].parent >= 0)
    {
        if (reverseCount >= ROUTE24_SCENE_ROUTE_CAPACITY - 1)
            return FALSE;
        reversePath[reverseCount++] = sRoute24SceneNodes[destinationNode].direction;
        destinationNode = sRoute24SceneNodes[destinationNode].parent;
    }

    while (reverseCount != 0)
    {
        enum Direction direction = reversePath[--reverseCount];

        if (!GoldenYellow_Route24AppendMovement(movementCount, GetWalkNormalMovementAction(direction)))
            return FALSE;
    }

    *startX = targetX;
    *startY = targetY;
    return TRUE;
}

static bool32 GoldenYellow_Route24AppendPreset(const struct ObjectEvent *actor,
                                                s16 *x,
                                                s16 *y,
                                                const enum Direction *directions,
                                                u8 directionCount,
                                                u8 *movementCount)
{
    u8 i;

    for (i = 0; i < directionCount; i++)
    {
        if (!GoldenYellow_Route24AppendValidatedStep(actor, x, y, directions[i], movementCount))
            return FALSE;
    }

    return TRUE;
}

static bool32 GoldenYellow_BuildRoute24PartnerStage(const struct ObjectEvent *partner,
                                                     enum GoldenYellowRoute24SceneRoute route,
                                                     enum Direction *finalFacing)
{
    static const enum Direction sWeakOrder[] =
    {
        DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_NORTH,
    };
    static const enum Direction sHealedOrder[] =
    {
        DIR_WEST, DIR_EAST, DIR_SOUTH, DIR_NORTH,
    };
    static const enum Direction sWeakLeftNorth[] =
    {
        DIR_EAST, DIR_NORTH, DIR_NORTH, DIR_NORTH,
    };
    static const enum Direction sHealedLeftNorth[] =
    {
        DIR_EAST, DIR_NORTH, DIR_NORTH, DIR_NORTH, DIR_NORTH, DIR_WEST,
    };
    static const enum Direction sHealedRightNorth[] =
    {
        DIR_WEST, DIR_NORTH, DIR_NORTH, DIR_EAST, DIR_NORTH, DIR_NORTH, DIR_WEST,
    };
    static const enum Direction sHealedFromEast[] =
    {
        DIR_NORTH, DIR_WEST,
    };
    static const enum Direction sRightLateralAroundPlayer[] =
    {
        DIR_SOUTH, DIR_WEST, DIR_WEST, DIR_NORTH,
        DIR_NORTH, DIR_EAST, DIR_NORTH,
    };
    struct ObjectEvent *charmander = GoldenYellow_FindRoute24Charmander();
    const struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    const enum Direction *directionOrder;
    s16 x;
    s16 y;
    s16 targetX;
    s16 targetY;
    bool32 lateralApproach = FALSE;
    u8 count = 0;

    if (charmander == NULL)
        return FALSE;

    x = partner->currentCoords.x;
    y = partner->currentCoords.y;
    if (route == GY_ROUTE24_ROUTE_PARTNER_WEAK)
    {
        targetX = charmander->currentCoords.x + 1;
        targetY = charmander->currentCoords.y;
        directionOrder = sWeakOrder;
        *finalFacing = DIR_WEST;

        if (player->currentCoords.x == charmander->currentCoords.x
         && player->currentCoords.y == charmander->currentCoords.y + 2
         && x == player->currentCoords.x
         && y == player->currentCoords.y + 1)
        {
            if (!GoldenYellow_Route24AppendPreset(partner, &x, &y,
                                                   sWeakLeftNorth,
                                                   ARRAY_COUNT(sWeakLeftNorth),
                                                   &count))
                return FALSE;
            goto finish;
        }
    }
    else
    {
        targetX = charmander->currentCoords.x;
        targetY = charmander->currentCoords.y - 1;
        directionOrder = sHealedOrder;
        *finalFacing = DIR_SOUTH;

        if (player->currentCoords.y == charmander->currentCoords.y + 2
         && x == player->currentCoords.x
         && y == player->currentCoords.y + 1)
        {
            if (player->currentCoords.x == charmander->currentCoords.x)
            {
                if (!GoldenYellow_Route24AppendPreset(partner, &x, &y,
                                                       sHealedLeftNorth,
                                                       ARRAY_COUNT(sHealedLeftNorth),
                                                       &count))
                    return FALSE;
                goto finish;
            }
            if (player->currentCoords.x == charmander->currentCoords.x + 1)
            {
                if (!GoldenYellow_Route24AppendPreset(partner, &x, &y,
                                                       sHealedRightNorth,
                                                       ARRAY_COUNT(sHealedRightNorth),
                                                       &count))
                    return FALSE;
                goto finish;
            }
        }
    }

    // Entering the right trigger from the east places Pikachu directly east of
    // the player. Use the exact accepted first-scene lane around the player;
    // the healed scene then continues north and west into its final position.
    if (player->currentCoords.x == charmander->currentCoords.x + 1
     && player->currentCoords.y == charmander->currentCoords.y + 2
     && x == player->currentCoords.x + 1
     && y == player->currentCoords.y)
    {
        if (!GoldenYellow_Route24AppendPreset(partner, &x, &y,
                                               sRightLateralAroundPlayer,
                                               ARRAY_COUNT(sRightLateralAroundPlayer),
                                               &count))
            return FALSE;
        if (route == GY_ROUTE24_ROUTE_PARTNER_HEALED
         && !GoldenYellow_Route24AppendPreset(partner, &x, &y,
                                               sHealedFromEast,
                                               ARRAY_COUNT(sHealedFromEast),
                                               &count))
            return FALSE;
        goto finish;
    }

    // A lateral trigger entry leaves Pikachu beside the player. Move one tile
    // south first so the route cannot cut through the player's occupied tile.
    if (y == player->currentCoords.y
     && (x == player->currentCoords.x - 1 || x == player->currentCoords.x + 1))
    {
        if (!GoldenYellow_Route24AppendValidatedStep(partner, &x, &y, DIR_SOUTH, &count))
            return FALSE;
        lateralApproach = TRUE;
    }

    // The healed lateral presentation must use the same proven approach around
    // the player as the weak scene. From Charmander's east side, Pikachu then
    // takes the two clear tiles north and west into the healed staging spot.
    if (route == GY_ROUTE24_ROUTE_PARTNER_HEALED && lateralApproach)
    {
        s16 eastX = charmander->currentCoords.x + 1;
        s16 eastY = charmander->currentCoords.y;

        if (!GoldenYellow_Route24AppendPathSegment(partner, &x, &y,
                                                    eastX, eastY,
                                                    sWeakOrder, &count)
         || !GoldenYellow_Route24AppendPreset(partner, &x, &y,
                                               sHealedFromEast,
                                               ARRAY_COUNT(sHealedFromEast),
                                               &count))
            return FALSE;
        goto finish;
    }

    if (!GoldenYellow_Route24AppendPathSegment(partner, &x, &y,
                                                targetX, targetY,
                                                directionOrder, &count))
        return FALSE;

finish:
    if (x != targetX || y != targetY
     || !GoldenYellow_Route24AppendMovement(&count, GetFaceDirectionMovementAction(*finalFacing)))
        return FALSE;

    sRoute24SceneMovement[count] = MOVEMENT_ACTION_STEP_END;
    return TRUE;
}

static bool32 GoldenYellow_BuildRoute24DamianRoute(const struct ObjectEvent *damian,
                                                    enum GoldenYellowRoute24SceneRoute route,
                                                    enum GoldenYellowRoute24DamianLane lane,
                                                    enum Direction *finalFacing)
{
    static const enum Direction sDamianOrder[] =
    {
        DIR_NORTH, DIR_WEST, DIR_EAST, DIR_SOUTH,
    };
    struct ObjectEvent *charmander = GoldenYellow_FindRoute24Charmander();
    s16 x = damian->currentCoords.x;
    s16 y = damian->currentCoords.y;
    s16 targetX;
    s16 targetY;
    u8 count = 0;

    if (route == GY_ROUTE24_ROUTE_DAMIAN_APPROACH)
    {
        if (charmander == NULL)
            return FALSE;
        if (lane == GY_ROUTE24_DAMIAN_LANE_SOUTH)
        {
            targetX = charmander->currentCoords.x;
            targetY = charmander->currentCoords.y + 1;
            *finalFacing = DIR_NORTH;
        }
        else
        {
            targetX = charmander->currentCoords.x + 1;
            targetY = charmander->currentCoords.y;
            *finalFacing = DIR_WEST;
        }
    }
    else
    {
        if (lane == GY_ROUTE24_DAMIAN_LANE_SOUTH)
        {
            targetX = x;
            targetY = y - 1;
        }
        else
        {
            targetX = x - 1;
            targetY = y;
        }
        *finalFacing = DIR_SOUTH;
    }

    if (!GoldenYellow_Route24AppendPathSegment(damian, &x, &y,
                                                targetX, targetY,
                                                sDamianOrder, &count)
     || !GoldenYellow_Route24AppendMovement(&count, GetFaceDirectionMovementAction(*finalFacing)))
        return FALSE;

    sRoute24SceneMovement[count] = MOVEMENT_ACTION_STEP_END;
    return TRUE;
}

static bool32 GoldenYellow_BuildRoute24PartnerRejoin(const struct ObjectEvent *partner,
                                                      enum Direction *finalFacing)
{
    static const enum Direction sRejoinOrder[] =
    {
        DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_NORTH,
    };
    const struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    enum GoldenYellowRoute24PlayerSide playerSide = VarGet(VAR_TEMP_C);
    s16 candidateX[4];
    s16 candidateY[4];
    s16 startX = partner->currentCoords.x;
    s16 startY = partner->currentCoords.y;
    u8 i;

    // The canonical south interaction rejoins below the player. Right- and
    // left-side interactions first try the corresponding behind-player tile.
    // Remaining adjacent tiles are collision-safe fallbacks, never teleports.
    if (playerSide == GY_ROUTE24_PLAYER_SIDE_RIGHT)
    {
        candidateX[0] = player->currentCoords.x + 1;
        candidateY[0] = player->currentCoords.y;
    }
    else if (playerSide == GY_ROUTE24_PLAYER_SIDE_LEFT)
    {
        candidateX[0] = player->currentCoords.x - 1;
        candidateY[0] = player->currentCoords.y;
    }
    else
    {
        candidateX[0] = player->currentCoords.x;
        candidateY[0] = player->currentCoords.y + 1;
    }

    candidateX[1] = player->currentCoords.x + 1;
    candidateY[1] = player->currentCoords.y;
    candidateX[2] = player->currentCoords.x - 1;
    candidateY[2] = player->currentCoords.y;
    candidateX[3] = player->currentCoords.x;
    candidateY[3] = player->currentCoords.y - 1;

    for (i = 0; i < ARRAY_COUNT(candidateX); i++)
    {
        s16 x = startX;
        s16 y = startY;
        u8 count = 0;

        if (GoldenYellow_Route24TileHasObject(partner, candidateX[i], candidateY[i]))
            continue;
        if (!GoldenYellow_Route24AppendPathSegment(partner, &x, &y,
                                                    candidateX[i], candidateY[i],
                                                    sRejoinOrder, &count))
            continue;

        if (candidateX[i] < player->currentCoords.x)
            *finalFacing = DIR_EAST;
        else if (candidateX[i] > player->currentCoords.x)
            *finalFacing = DIR_WEST;
        else if (candidateY[i] < player->currentCoords.y)
            *finalFacing = DIR_SOUTH;
        else
            *finalFacing = DIR_NORTH;

        if (!GoldenYellow_Route24AppendMovement(&count, GetFaceDirectionMovementAction(*finalFacing)))
            return FALSE;

        sRoute24SceneMovement[count] = MOVEMENT_ACTION_STEP_END;
        return TRUE;
    }

    return FALSE;
}

static bool32 GoldenYellow_BuildRoute24PartnerCancelRejoin(const struct ObjectEvent *partner,
                                                            enum Direction *finalFacing)
{
    static const enum Direction sCancelOrder[] =
    {
        DIR_EAST, DIR_WEST, DIR_NORTH, DIR_SOUTH,
    };
    const struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    enum Direction playerFacing = GetPlayerFacingDirection();
    enum Direction behindDirection;
    s16 x = partner->currentCoords.x;
    s16 y = partner->currentCoords.y;
    s16 targetX = player->currentCoords.x;
    s16 targetY = player->currentCoords.y;
    s16 frontX = player->currentCoords.x;
    s16 frontY = player->currentCoords.y;
    u8 count = 0;
    bool32 routeBuilt;

    switch (playerFacing)
    {
    case DIR_NORTH:
        behindDirection = DIR_SOUTH;
        break;
    case DIR_SOUTH:
        behindDirection = DIR_NORTH;
        break;
    case DIR_WEST:
        behindDirection = DIR_EAST;
        break;
    case DIR_EAST:
        behindDirection = DIR_WEST;
        break;
    default:
        return FALSE;
    }

    MoveCoords(behindDirection, &targetX, &targetY);
    MoveCoords(playerFacing, &frontX, &frontY);

    // Cancellation must never route Pikachu through the player or through the
    // tile in front of a departing player. Normal follower ownership is
    // restored only after Pikachu reaches the true behind-player tile.
    sRoute24SceneHasForbiddenTile = TRUE;
    sRoute24SceneForbiddenX = frontX;
    sRoute24SceneForbiddenY = frontY;
    routeBuilt = GoldenYellow_Route24AppendPathSegment(partner, &x, &y,
                                                        targetX, targetY,
                                                        sCancelOrder, &count);
    sRoute24SceneHasForbiddenTile = FALSE;

    if (!routeBuilt
     || !GoldenYellow_Route24AppendMovement(&count, GetFaceDirectionMovementAction(playerFacing)))
        return FALSE;

    *finalFacing = playerFacing;
    sRoute24SceneMovement[count] = MOVEMENT_ACTION_STEP_END;
    return TRUE;
}

static bool8 GoldenYellow_WaitForRoute24SceneRoute(void)
{
    struct ObjectEvent *object;

    if (!GoldenYellow_IsOnRoute24() || sRoute24SceneLocalId == 0)
        return TRUE;

    if (!ScriptMovement_IsObjectMovementFinished(sRoute24SceneLocalId,
                                                  gSaveBlock1Ptr->location.mapNum,
                                                  gSaveBlock1Ptr->location.mapGroup))
        return FALSE;

    object = GoldenYellow_FindRoute24ObjectByLocalId(sRoute24SceneLocalId);
    if (object != NULL)
    {
        ObjectEventClearHeldMovementIfFinished(object);
        UnfreezeObjectEvent(object);
    }

    sRoute24SceneLocalId = 0;
    return TRUE;
}

static void GoldenYellow_StartRoute24SceneMovement(struct ScriptContext *ctx,
                                                    struct ObjectEvent *object,
                                                    bool32 routeBuilt)
{
    gSpecialVar_Result = FALSE;
    sRoute24SceneLocalId = 0;

    if (!routeBuilt || object == NULL)
        return;

    sRoute24SceneLocalId = object->localId;
    if (ScriptMovement_StartObjectMovementScript(sRoute24SceneLocalId,
                                                  gSaveBlock1Ptr->location.mapNum,
                                                  gSaveBlock1Ptr->location.mapGroup,
                                                  sRoute24SceneMovement))
    {
        sRoute24SceneLocalId = 0;
        return;
    }

    gSpecialVar_Result = TRUE;
    SetupNativeScript(ctx, GoldenYellow_WaitForRoute24SceneRoute);
    ctx->waitAfterCallNative = TRUE;
}

void GoldenYellow_ParkRoute24PartnerFollower(struct ScriptContext *ctx)
{
    struct ObjectEvent *follower = GetFollowerObject();

    (void)ctx;
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (!GoldenYellow_IsCanonicalRoute24Partner(follower))
        return;

    ClearObjectEventMovement(follower, &gSprites[follower->spriteId]);
    UnfreezeObjectEvent(follower);
    SetTrainerMovementType(follower, MOVEMENT_TYPE_NONE);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}

void GoldenYellow_StartRoute24PartnerStage(struct ScriptContext *ctx)
{
    struct ObjectEvent *follower = GetFollowerObject();
    enum Direction finalFacing = DIR_NONE;
    enum GoldenYellowRoute24SceneRoute route = gSpecialVar_0x8004;
    bool32 routeBuilt = FALSE;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (GoldenYellow_IsCanonicalRoute24Partner(follower)
     && (route == GY_ROUTE24_ROUTE_PARTNER_WEAK
      || route == GY_ROUTE24_ROUTE_PARTNER_HEALED))
        routeBuilt = GoldenYellow_BuildRoute24PartnerStage(follower, route, &finalFacing);

    GoldenYellow_StartRoute24SceneMovement(ctx, follower, routeBuilt);
}

void GoldenYellow_StartRoute24DamianRoute(struct ScriptContext *ctx)
{
    struct ObjectEvent *damian = GoldenYellow_FindRoute24ObjectByLocalId(gSpecialVar_0x8006);
    enum Direction finalFacing = DIR_NONE;
    enum GoldenYellowRoute24SceneRoute route = gSpecialVar_0x8004;
    enum GoldenYellowRoute24DamianLane lane = gSpecialVar_0x8005;
    bool32 routeBuilt = FALSE;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (damian != NULL
     && (route == GY_ROUTE24_ROUTE_DAMIAN_APPROACH
      || route == GY_ROUTE24_ROUTE_DAMIAN_CENTER)
     && (lane == GY_ROUTE24_DAMIAN_LANE_RIGHT
      || lane == GY_ROUTE24_DAMIAN_LANE_SOUTH))
        routeBuilt = GoldenYellow_BuildRoute24DamianRoute(damian, route, lane, &finalFacing);

    GoldenYellow_StartRoute24SceneMovement(ctx, damian, routeBuilt);
}

void GoldenYellow_StartRoute24PartnerRejoin(struct ScriptContext *ctx)
{
    struct ObjectEvent *follower = GetFollowerObject();
    enum Direction finalFacing = DIR_NONE;
    enum GoldenYellowRoute24SceneRoute route = gSpecialVar_0x8004;
    bool32 routeBuilt = FALSE;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (GoldenYellow_IsCanonicalRoute24Partner(follower))
    {
        if (route == GY_ROUTE24_ROUTE_PARTNER_CANCEL_REJOIN)
            routeBuilt = GoldenYellow_BuildRoute24PartnerCancelRejoin(follower, &finalFacing);
        else if (route == GY_ROUTE24_ROUTE_PARTNER_REJOIN)
            routeBuilt = GoldenYellow_BuildRoute24PartnerRejoin(follower, &finalFacing);
    }

    GoldenYellow_StartRoute24SceneMovement(ctx, follower, routeBuilt);
}

void GoldenYellow_RestoreRoute24PartnerFollower(struct ScriptContext *ctx)
{
    struct ObjectEvent *follower = GetFollowerObject();

    (void)ctx;
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (!GoldenYellow_IsCanonicalRoute24Partner(follower))
        return;

    GoldenYellow_NormalizeBillPartnerFollower(follower);
}

static bool32 GoldenYellow_FacePartnerTowardRoute24Charmander(void)
{
    struct ObjectEvent *partnerObject = GetFollowerObject();
    struct ObjectEvent *charmanderObject = NULL;
    enum Direction direction;
    u32 i;

    if (!GoldenYellow_IsOnRoute24()
     || partnerObject == NULL
     || !partnerObject->active)
        return FALSE;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].active
         && gObjectEvents[i].mapGroup == gSaveBlock1Ptr->location.mapGroup
         && gObjectEvents[i].mapNum == gSaveBlock1Ptr->location.mapNum
         && gObjectEvents[i].graphicsId == OBJ_EVENT_GFX_SPECIES(CHARMANDER))
        {
            charmanderObject = &gObjectEvents[i];
            break;
        }
    }

    if (charmanderObject == NULL)
        return FALSE;

    if (charmanderObject->currentCoords.x < partnerObject->currentCoords.x)
        direction = DIR_WEST;
    else if (charmanderObject->currentCoords.x > partnerObject->currentCoords.x)
        direction = DIR_EAST;
    else if (charmanderObject->currentCoords.y < partnerObject->currentCoords.y)
        direction = DIR_NORTH;
    else if (charmanderObject->currentCoords.y > partnerObject->currentCoords.y)
        direction = DIR_SOUTH;
    else
        return FALSE;

    ObjectEventClearHeldMovementIfActive(partnerObject);
    UnfreezeObjectEvent(partnerObject);
    ObjectEventTurn(partnerObject, direction);
    return TRUE;
}

static bool8 GoldenYellow_WaitForCharmanderPartnerConcern(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_ClearPartnerPikachuReactionObject();
    GoldenYellow_FacePartnerTowardRoute24Charmander();
    return TRUE;
}

void GoldenYellow_StartCharmanderPartnerSceneReaction(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    u8 reaction = gSpecialVar_0x8004;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    GoldenYellow_ClearPartnerPikachuReactionObject();

    if (!GoldenYellow_IsOnRoute24()
     || partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER
     || GetFollowerObject() == NULL
     || (reaction != GY_PARTNER_REACTION_UNHAPPY
      && reaction != GY_PARTNER_REACTION_STRONG_HAPPINESS))
        return;

    if (reaction == GY_PARTNER_REACTION_UNHAPPY
     && !GoldenYellow_FacePartnerTowardRoute24Charmander())
        return;

    if (!GoldenYellow_StartPartnerPikachuReaction(reaction))
        return;

    if (reaction == GY_PARTNER_REACTION_UNHAPPY)
        SetupNativeScript(ctx, GoldenYellow_WaitForCharmanderPartnerConcern);
    else
        SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
}

void GoldenYellow_StartBillPartnerSceneReaction(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    u8 reaction = gSpecialVar_0x8004;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    GoldenYellow_ClearPartnerPikachuReactionObject();

    if (!GoldenYellow_IsInBillSeaCottage()
     || partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER
     || GoldenYellow_FindBillPartnerSceneObject() == NULL
     || (reaction != GY_PARTNER_REACTION_BILL_CONFUSED
      && reaction != GY_PARTNER_REACTION_BILL_SHOCKED)
     || !GoldenYellow_StartPartnerPikachuReaction(reaction))
        return;

    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
}

void GoldenYellow_StartBillPartnerEntryApproach(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    struct ObjectEvent *partnerObject;
    struct ObjectEvent *billObject;
    struct ObjectEvent *player;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (!GoldenYellow_IsInBillSeaCottage()
     || partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER)
        return;

    partnerObject = GoldenYellow_FindBillPartnerSceneObject();
    billObject = GoldenYellow_FindTransformedBillObject();
    if (partnerObject == NULL || billObject == NULL)
        return;

    player = &gObjectEvents[gPlayerAvatar.objectEventId];

    // Reuse the accepted Fan Club/Pewter lifecycle: keep the actual following-
    // Pokemon object alive, park it before staging, then establish the visible
    // right-of-player pose before the attention beat begins.
    GoldenYellow_ParkBillPartnerFollower(partnerObject);
    MoveObjectEventToMapCoords(partnerObject, player->currentCoords.x + 1, player->currentCoords.y);
    ObjectEventTurn(partnerObject, DIR_NORTH);

    if (!GoldenYellow_BuildBillPartnerApproachMovement(partnerObject, billObject))
    {
        GoldenYellow_NormalizeBillPartnerFollower(partnerObject);
        return;
    }

    sBillPartnerApproachLocalId = partnerObject->localId;
    PlaySE(SE_PIN);
    if (ScriptMovement_StartObjectMovementScript(sBillPartnerApproachLocalId,
                                                  gSaveBlock1Ptr->location.mapNum,
                                                  gSaveBlock1Ptr->location.mapGroup,
                                                  sBillPartnerApproachMovement))
    {
        sBillPartnerApproachLocalId = 0;
        GoldenYellow_NormalizeBillPartnerFollower(partnerObject);
        return;
    }

    SetupNativeScript(ctx, GoldenYellow_WaitForBillPartnerEntryApproach);
    ctx->waitAfterCallNative = TRUE;
}

static bool8 GoldenYellow_WaitForBillPartnerRejoin(void)
{
    struct ObjectEvent *follower;

    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_ClearPartnerPikachuReactionObject();

    if (!GoldenYellow_IsInBillSeaCottage()
     || !FlagGet(FLAG_HELPED_BILL_IN_SEA_COTTAGE))
        return TRUE;

    follower = GoldenYellow_FindBillPartnerSceneObject();
    GoldenYellow_NormalizeBillPartnerFollower(follower);

    // Keep the retired map-owned Bill Pikachu hidden and make sure the generic
    // follower is never suppressed. No object is removed or recreated here.
    FlagSet(FLAG_TEMP_5);
    FlagClear(FLAG_TEMP_HIDE_FOLLOWER);
    VarSet(VAR_GY_BILL_PARTNER_RELEASED, 1);
    return TRUE;
}

static bool32 GoldenYellow_IsBillPartnerInteractionObject(void)
{
    struct ObjectEvent *selectedObject;
    struct ObjectEvent *follower;

    if (!GoldenYellow_IsInBillSeaCottage()
     || gSelectedObjectEvent >= OBJECT_EVENTS_COUNT)
        return FALSE;

    selectedObject = &gObjectEvents[gSelectedObjectEvent];
    follower = GetFollowerObject();

    // Bill now uses the same live following-Pokemon object throughout the scene.
    // Require that exact object rather than accepting any Pikachu-shaped map NPC.
    return follower != NULL
        && follower->active
        && selectedObject == follower;
}

static bool32 GoldenYellow_IsPokemonTowerPartnerInteractionMap(void)
{
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_POKEMON_TOWER_1F))
        return FALSE;

    // Yellow applies the authored afraid reaction to direct Partner talk on
    // every Pokemon Tower floor. List each FRLG map explicitly instead of
    // depending on map-number contiguity so later map-table edits cannot widen
    // or silently break this story-specific precedence rule.
    switch (gSaveBlock1Ptr->location.mapNum)
    {
    case MAP_NUM(MAP_POKEMON_TOWER_1F):
    case MAP_NUM(MAP_POKEMON_TOWER_2F):
    case MAP_NUM(MAP_POKEMON_TOWER_3F):
    case MAP_NUM(MAP_POKEMON_TOWER_4F):
    case MAP_NUM(MAP_POKEMON_TOWER_5F):
    case MAP_NUM(MAP_POKEMON_TOWER_6F):
    case MAP_NUM(MAP_POKEMON_TOWER_7F):
        return TRUE;
    default:
        return FALSE;
    }
}

static enum GoldenYellowPartnerInteractionRoute GoldenYellow_ResolvePartnerPikachuFieldInteraction(
    struct Pokemon *partner,
    struct ObjectEvent *follower,
    u8 *reaction)
{
    u32 status;

    // P7E authoritative priority:
    //   Story > Status > Area > P6 one-shot modifier > P5 mood/friendship.
    // Resolve exactly one owner before dispatch so lower-priority systems cannot
    // consume state or override an authored Yellow reaction.

    // Story: Bill owns the canonical follower while it is parked for the Sea
    // Cottage sequence, so resolve that authored reaction before lower tiers.
    if (GoldenYellow_IsBillPartnerInteractionObject())
    {
        *reaction = FlagGet(FLAG_HELPED_BILL_IN_SEA_COTTAGE)
                  ? GY_PARTNER_REACTION_BILL_POST_STATE
                  : GY_PARTNER_REACTION_BILL_INTERMEDIATE;
        return GY_PARTNER_INTERACTION_STORY_BILL;
    }

    if (follower == NULL || !follower->active)
        return GY_PARTNER_INTERACTION_NONE;

    // Story: P7C parked Fan Club Partner and P7A authored Pewter sleep/wake
    // retain ownership over every general status/area/modifier/mood reaction.
    if (GoldenYellow_IsFanClubPartnerParked(partner))
    {
        *reaction = GY_PARTNER_REACTION_FAN_CLUB_MAX_AFFECTION;
        return GY_PARTNER_INTERACTION_STORY_FAN_CLUB;
    }

    if (GoldenYellow_IsPewterPartnerSleepActive(partner))
        return GY_PARTNER_INTERACTION_STORY_PEWTER_WAKE;

    // Status: mirror Yellow's selector ordering. True battle sleep uses the
    // normal sleeping reaction; any other nonzero primary status uses Emotion28.
    // P7A's authored sleep is intentionally handled above and never writes this
    // field, so the two lifecycles remain independent.
    status = GetMonData(partner, MON_DATA_STATUS);
    if (status & STATUS1_SLEEP)
    {
        *reaction = GY_PARTNER_REACTION_SLEEPING;
        return GY_PARTNER_INTERACTION_STATUS;
    }

    if (status != STATUS1_NONE)
    {
        *reaction = GY_PARTNER_REACTION_STATUS_SICK;
        return GY_PARTNER_INTERACTION_STATUS;
    }

    // Area: P7D Tower fear is repeatable and does not consume P6 state.
    if (GoldenYellow_IsPokemonTowerPartnerInteractionMap())
    {
        *reaction = GY_PARTNER_REACTION_TOWER_AFRAID;
        return GY_PARTNER_INTERACTION_AREA_TOWER;
    }

    // P6: query only after Story/Status/Area all lose. Consumption happens only
    // after the selected reaction successfully starts in the dispatcher below.
    if (GoldenYellow_TryGetPartnerPikachuOneShotReaction(partner, reaction))
        return GY_PARTNER_INTERACTION_ONE_SHOT;

    // P5 is the final fallback for normal canonical Partner conversation.
    *reaction = GoldenYellow_SelectPartnerTalkReaction(partner);
    return GY_PARTNER_INTERACTION_MOOD;
}

void GoldenYellow_RestorePewterPartnerSleepNative(struct ScriptContext *ctx)
{
    (void)ctx;
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    GoldenYellow_RestorePewterPartnerSleepOnFollower();
}

void GoldenYellow_StartPewterPartnerSleepReaction(struct ScriptContext *ctx)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    gSpecialVar_Result = FALSE;

    // The saved sleep state is prepared immediately before this call. Commit the
    // authored scene only if the P3 reaction task successfully takes ownership;
    // otherwise normalize the follower and roll the scene state back to idle so
    // a later Jigglypuff interaction can safely retry.
    if (!GoldenYellow_StartPartnerPikachuReaction(GY_PARTNER_REACTION_SLEEPING))
    {
        GoldenYellow_CancelPewterPartnerSleepOnFollower();
        return;
    }

    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
    gSpecialVar_Result = TRUE;
}
static bool32 GoldenYellow_StartPewterPartnerWake(struct ScriptContext *ctx, struct Pokemon *partner)
{
    if (!GoldenYellow_IsPewterPartnerSleepActive(partner))
        return FALSE;

    if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(GY_PARTNER_REACTION_PEWTER_JIGGLYPUFF))
        return FALSE;

    // Direct A-button wake keeps the existing ACTIVE state until the reaction
    // finishes. Item/Poke Flute wake already enters WAKE_PENDING before this
    // function is reached through the map-resume path. Do not promote direct
    // field talk to WAKE_PENDING, otherwise the on-frame script becomes a second
    // owner of the same wake transition.
    SetupNativeScript(ctx, GoldenYellow_WaitForPewterPartnerWake);
    ctx->waitAfterCallNative = TRUE;
    return TRUE;
}

void GoldenYellow_ResumePendingPewterPartnerWake(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (!GoldenYellow_IsPewterPartnerWakePending())
        return;

    if (partner == NULL || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER)
    {
        GoldenYellow_CancelPewterPartnerSleepOnFollower();
        return;
    }

    GoldenYellow_StartPewterPartnerWake(ctx, partner);
}

void GoldenYellow_TryPartnerPikachuFieldInteraction(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    struct ObjectEvent *follower;
    enum GoldenYellowPartnerInteractionRoute route;
    u8 reaction = GY_PARTNER_REACTION_EMPTY;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    gSpecialVar_Result = FALSE;
    GoldenYellow_ClearPartnerPikachuReactionObject();

    if (partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER)
        return;

    follower = GetFollowerObject();
    route = GoldenYellow_ResolvePartnerPikachuFieldInteraction(partner, follower, &reaction);
    if (route == GY_PARTNER_INTERACTION_NONE)
        return;

    gSpecialVar_Result = TRUE;

    switch (route)
    {
    case GY_PARTNER_INTERACTION_STORY_BILL:
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
            return;

        if (FlagGet(FLAG_HELPED_BILL_IN_SEA_COTTAGE))
            SetupNativeScript(ctx, GoldenYellow_WaitForBillPartnerRejoin);
        else
            SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
        ctx->waitAfterCallNative = TRUE;
        return;

    case GY_PARTNER_INTERACTION_STORY_FAN_CLUB:
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
            return;

        SetupNativeScript(ctx, GoldenYellow_WaitForFanClubPartnerRejoin);
        ctx->waitAfterCallNative = TRUE;
        return;

    case GY_PARTNER_INTERACTION_STORY_PEWTER_WAKE:
        GoldenYellow_StartPewterPartnerWake(ctx, partner);
        return;

    case GY_PARTNER_INTERACTION_ONE_SHOT:
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
            return;

        GoldenYellow_ConsumePartnerPikachuOneShotReaction(partner);
        break;

    case GY_PARTNER_INTERACTION_STATUS:
    case GY_PARTNER_INTERACTION_AREA_TOWER:
    case GY_PARTNER_INTERACTION_MOOD:
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
            return;
        break;

    case GY_PARTNER_INTERACTION_NONE:
    default:
        return;
    }

    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
}