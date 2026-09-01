#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "golden_yellow_partner_fan_club.h"
#include "golden_yellow_partner_reaction.h"
#include "golden_yellow_partner_state.h"
#include "pokemon.h"
#include "script.h"
#include "script_movement.h"
#include "sound.h"
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
static u8 sBillInteractionObjectEventId = OBJECT_EVENTS_COUNT;

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
    u32 i;

    if (!GoldenYellow_IsInBillSeaCottage())
        return NULL;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *object = &gObjectEvents[i];

        if (!object->active
         || object == follower
         || object->localId == OBJ_EVENT_ID_FOLLOWER
         || object->graphicsId != OBJ_EVENT_GFX_PIKACHU_FRLG
         || object->mapGroup != gSaveBlock1Ptr->location.mapGroup
         || object->mapNum != gSaveBlock1Ptr->location.mapNum)
            continue;

        return object;
    }

    return NULL;
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

    // Reproduce the accepted Fan Club entrance pose: Partner is visibly beside
    // the player on the player's right before the attention beat begins.
    ObjectEventClearHeldMovementIfActive(partnerObject);
    UnfreezeObjectEvent(partnerObject);
    MoveObjectEventToMapCoords(partnerObject, player->currentCoords.x + 1, player->currentCoords.y);
    ObjectEventTurn(partnerObject, DIR_NORTH);

    if (!GoldenYellow_BuildBillPartnerApproachMovement(partnerObject, billObject))
        return;

    sBillPartnerApproachLocalId = partnerObject->localId;
    PlaySE(SE_PIN);
    if (ScriptMovement_StartObjectMovementScript(sBillPartnerApproachLocalId,
                                                  gSaveBlock1Ptr->location.mapNum,
                                                  gSaveBlock1Ptr->location.mapGroup,
                                                  sBillPartnerApproachMovement))
    {
        sBillPartnerApproachLocalId = 0;
        return;
    }

    SetupNativeScript(ctx, GoldenYellow_WaitForBillPartnerEntryApproach);
    ctx->waitAfterCallNative = TRUE;
}

static bool8 GoldenYellow_WaitForBillPartnerRejoin(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_ClearPartnerPikachuReactionObject();

    if (!GoldenYellow_IsInBillSeaCottage()
     || !FlagGet(FLAG_HELPED_BILL_IN_SEA_COTTAGE))
    {
        sBillInteractionObjectEventId = OBJECT_EVENTS_COUNT;
        return TRUE;
    }

    // The first post-restoration A-button reaction is the explicit release beat.
    // Hide/remove the scene-owned Pikachu and hand authority back to the normal
    // following-Pokemon object. Later visits already use the normal follower, so
    // this handoff becomes a harmless no-op while Emotion31 remains repeatable.
    if (FlagGet(FLAG_TEMP_HIDE_FOLLOWER))
    {
        if (sBillInteractionObjectEventId < OBJECT_EVENTS_COUNT
         && gObjectEvents[sBillInteractionObjectEventId].active
         && gObjectEvents[sBillInteractionObjectEventId].localId != OBJ_EVENT_ID_FOLLOWER)
            RemoveObjectEvent(&gObjectEvents[sBillInteractionObjectEventId]);

        FlagSet(FLAG_TEMP_5);
        FlagClear(FLAG_TEMP_HIDE_FOLLOWER);
        UpdateFollowingPokemon();
    }

    VarSet(VAR_GY_BILL_PARTNER_RELEASED, 1);
    sBillInteractionObjectEventId = OBJECT_EVENTS_COUNT;
    return TRUE;
}

static bool32 GoldenYellow_IsBillPartnerInteractionObject(void)
{
    struct ObjectEvent *selectedObject;

    if (!GoldenYellow_IsInBillSeaCottage()
     || gSelectedObjectEvent >= OBJECT_EVENTS_COUNT)
        return FALSE;

    selectedObject = &gObjectEvents[gSelectedObjectEvent];

    // The map/location and selected-object test identifies either the dedicated
    // pre-release scene object or the normal follower used on later visits.
    // Canonical identity is still established separately below; graphics alone
    // never qualifies an ordinary Pikachu as the Partner.
    return selectedObject->active
        && selectedObject->graphicsId == OBJ_EVENT_GFX_PIKACHU_FRLG;
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

    // Story: Bill's scene-owned object must resolve before the generic follower
    // requirement because the accepted Sea Cottage scene can temporarily own a
    // dedicated visible Partner object.
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
        sBillInteractionObjectEventId = gSelectedObjectEvent;
        GoldenYellow_SetPartnerPikachuReactionObject(gSelectedObjectEvent);
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
        {
            GoldenYellow_ClearPartnerPikachuReactionObject();
            sBillInteractionObjectEventId = OBJECT_EVENTS_COUNT;
            return;
        }

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
