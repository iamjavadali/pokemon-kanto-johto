#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "golden_yellow_partner_fan_club.h"
#include "golden_yellow_partner_reaction.h"
#include "overworld.h"
#include "pokemon.h"
#include "script.h"
#include "script_movement.h"
#include "sprite.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/maps.h"
#include "constants/species.h"
#include "constants/vars.h"

// P7C owns the next two unused saved vars. The position is stored only after
// the automatic Emotion 29 arrival reaction completes, because that is the
// first player-controllable point where the parked scene can be saved.
#define VAR_GY_FAN_CLUB_PARTNER_STATE VAR_UNUSED_0x40FE
#define VAR_GY_FAN_CLUB_PARTNER_POS   VAR_UNUSED_0x40FF

#define FAN_CLUB_APPROACH_MOVEMENT_CAPACITY 24

enum GoldenYellowFanClubPartnerState
{
    GY_FAN_CLUB_PARTNER_INACTIVE = 0,
    GY_FAN_CLUB_PARTNER_APPROACHING,
    GY_FAN_CLUB_PARTNER_REACTING,
    GY_FAN_CLUB_PARTNER_PARKED,
};

static EWRAM_DATA u8 sFanClubApproachMovement[FAN_CLUB_APPROACH_MOVEMENT_CAPACITY];

static bool32 IsInVermilionFanClub(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_VERMILION_CITY_POKEMON_FAN_CLUB)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_VERMILION_CITY_POKEMON_FAN_CLUB);
}

static bool32 IsCanonicalFollowingPartner(struct Pokemon *partner)
{
    return partner != NULL
        && GetMonData(partner, MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER;
}

static void ClearFanClubPartnerState(void)
{
    VarSet(VAR_GY_FAN_CLUB_PARTNER_STATE, GY_FAN_CLUB_PARTNER_INACTIVE);
    VarSet(VAR_GY_FAN_CLUB_PARTNER_POS, 0);
}

static void NormalizeFanClubPartnerFollower(struct ObjectEvent *follower, bool32 reanchorIfDistant)
{
    if (follower == NULL || !follower->active)
        return;

    // Reuse the accepted P7A ownership handoff exactly: clear both held and
    // single movement state before changing movement type. Clearing only held
    // movement can strand the following-Pokemon object after an authored scene.
    ClearObjectEventMovement(follower, &gSprites[follower->spriteId]);
    UnfreezeObjectEvent(follower);

    if (reanchorIfDistant)
    {
        struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
        s16 deltaX = player->currentCoords.x - follower->currentCoords.x;
        s16 deltaY = player->currentCoords.y - follower->currentCoords.y;

        if (deltaX < 0)
            deltaX = -deltaX;
        if (deltaY < 0)
            deltaY = -deltaY;

        if (deltaX + deltaY > 1)
            MoveObjectEventToMapCoords(follower, player->previousCoords.x, player->previousCoords.y);
    }

    SetTrainerMovementType(follower, MOVEMENT_TYPE_FOLLOW_PLAYER);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}

static void ParkFanClubPartnerFollower(struct ObjectEvent *follower)
{
    if (follower == NULL || !follower->active)
        return;

    // This is the same proven P7A transfer used by the Jigglypuff scene: the
    // actual following-Pokemon object remains alive, but FOLLOW_PLAYER releases
    // authority while the authored scene owns its position.
    ClearObjectEventMovement(follower, &gSprites[follower->spriteId]);
    UnfreezeObjectEvent(follower);
    SetTrainerMovementType(follower, MOVEMENT_TYPE_NONE);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}

static struct ObjectEvent *FindFanClubPikachuObject(void)
{
    struct ObjectEvent *follower = GetFollowerObject();
    u32 i;

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

static bool32 AppendFanClubApproachMovement(u8 *count, u8 movement)
{
    if (*count >= FAN_CLUB_APPROACH_MOVEMENT_CAPACITY - 1)
        return FALSE;

    sFanClubApproachMovement[(*count)++] = movement;
    return TRUE;
}

static bool32 BuildFanClubApproachMovement(const struct ObjectEvent *follower,
                                            const struct ObjectEvent *clubPikachu)
{
    s16 x;
    s16 y;
    s16 targetX;
    s16 targetY;
    u8 count = 0;

    if (follower == NULL || clubPikachu == NULL)
        return FALSE;

    x = follower->currentCoords.x;
    y = follower->currentCoords.y;
    targetX = clubPikachu->currentCoords.x - 1;
    targetY = clubPikachu->currentCoords.y;

    if (!AppendFanClubApproachMovement(&count, MOVEMENT_ACTION_EMOTE_EXCLAMATION_MARK)
     || !AppendFanClubApproachMovement(&count, MOVEMENT_ACTION_DELAY_16))
        return FALSE;

    while (x < targetX)
    {
        if (!AppendFanClubApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_RIGHT))
            return FALSE;
        x++;
    }
    while (x > targetX)
    {
        if (!AppendFanClubApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_LEFT))
            return FALSE;
        x--;
    }
    while (y < targetY)
    {
        if (!AppendFanClubApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_DOWN))
            return FALSE;
        y++;
    }
    while (y > targetY)
    {
        if (!AppendFanClubApproachMovement(&count, MOVEMENT_ACTION_WALK_NORMAL_UP))
            return FALSE;
        y--;
    }

    if (!AppendFanClubApproachMovement(&count, MOVEMENT_ACTION_FACE_RIGHT))
        return FALSE;

    sFanClubApproachMovement[count] = MOVEMENT_ACTION_STEP_END;
    return TRUE;
}

static void StoreFanClubPartnerPosition(const struct ObjectEvent *follower)
{
    u16 packedPosition;

    if (follower == NULL || !follower->active)
        return;

    packedPosition = (u8)follower->currentCoords.x
                   | ((u16)(u8)follower->currentCoords.y << 8);
    VarSet(VAR_GY_FAN_CLUB_PARTNER_POS, packedPosition);
}

static bool8 WaitForFanClubPartnerArrival(void)
{
    struct ObjectEvent *follower = GetFollowerObject();

    if (follower == NULL || !follower->active)
    {
        ClearFanClubPartnerState();
        return TRUE;
    }

    switch (VarGet(VAR_GY_FAN_CLUB_PARTNER_STATE))
    {
    case GY_FAN_CLUB_PARTNER_APPROACHING:
        if (!ScriptMovement_IsObjectMovementFinished(OBJ_EVENT_ID_FOLLOWER,
                                                      gSaveBlock1Ptr->location.mapNum,
                                                      gSaveBlock1Ptr->location.mapGroup))
            return FALSE;

        // ScriptMovement freezes objects at STEP_END. Re-open the same parked
        // follower before handing it to the accepted P3 reaction director.
        ObjectEventClearHeldMovementIfFinished(follower);
        UnfreezeObjectEvent(follower);
        VarSet(VAR_GY_FAN_CLUB_PARTNER_STATE, GY_FAN_CLUB_PARTNER_REACTING);

        if (!GoldenYellow_StartPartnerPikachuReaction(GY_PARTNER_REACTION_FAN_CLUB_AFFECTION))
        {
            NormalizeFanClubPartnerFollower(follower, TRUE);
            ClearFanClubPartnerState();
            return TRUE;
        }
        return FALSE;

    case GY_FAN_CLUB_PARTNER_REACTING:
        if (GoldenYellow_IsPartnerPikachuReactionActive())
            return FALSE;

        // P3 may finish on a portrait-facing pose. Re-park and face the club's
        // Pikachu so the controllable scene matches Yellow's visible intent.
        ParkFanClubPartnerFollower(follower);
        ObjectEventTurn(follower, DIR_EAST);
        StoreFanClubPartnerPosition(follower);
        VarSet(VAR_GY_FAN_CLUB_PARTNER_STATE, GY_FAN_CLUB_PARTNER_PARKED);
        return TRUE;

    default:
        return TRUE;
    }
}

void GoldenYellow_StartFanClubPartnerArrival(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    struct ObjectEvent *follower = GetFollowerObject();
    struct ObjectEvent *clubPikachu;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    gSpecialVar_Result = FALSE;

    // Match Yellow's authored guard: only the healthy canonical starter that is
    // actually following can break formation for the Fan Club scene.
    if (!IsInVermilionFanClub()
     || VarGet(VAR_GY_FAN_CLUB_PARTNER_STATE) != GY_FAN_CLUB_PARTNER_INACTIVE
     || !IsCanonicalFollowingPartner(partner)
     || GetMonData(partner, MON_DATA_HP) == 0
     || GetMonData(partner, MON_DATA_STATUS) != 0
     || follower == NULL
     || !follower->active)
        return;

    clubPikachu = FindFanClubPikachuObject();
    if (clubPikachu == NULL)
        return;

    // Transfer movement ownership before scripting the walk. This is the P7A
    // fix that prevents an in-flight FOLLOW_PLAYER step from deadlocking later
    // ScriptMovement commands.
    ParkFanClubPartnerFollower(follower);
    if (!BuildFanClubApproachMovement(follower, clubPikachu))
    {
        NormalizeFanClubPartnerFollower(follower, TRUE);
        return;
    }

    VarSet(VAR_GY_FAN_CLUB_PARTNER_STATE, GY_FAN_CLUB_PARTNER_APPROACHING);
    if (ScriptMovement_StartObjectMovementScript(OBJ_EVENT_ID_FOLLOWER,
                                                  gSaveBlock1Ptr->location.mapNum,
                                                  gSaveBlock1Ptr->location.mapGroup,
                                                  sFanClubApproachMovement))
    {
        NormalizeFanClubPartnerFollower(follower, TRUE);
        ClearFanClubPartnerState();
        return;
    }

    SetupNativeScript(ctx, WaitForFanClubPartnerArrival);
    ctx->waitAfterCallNative = TRUE;
    gSpecialVar_Result = TRUE;
}

void GoldenYellow_RestoreFanClubPartnerOnFollower(struct ScriptContext *ctx)
{
    struct Pokemon *partner;
    struct ObjectEvent *follower;
    u16 packedPosition;

    (void)ctx;
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (VarGet(VAR_GY_FAN_CLUB_PARTNER_STATE) != GY_FAN_CLUB_PARTNER_PARKED)
        return;

    partner = GetPartnerAwareFollowingMon();
    if (!IsInVermilionFanClub() || !IsCanonicalFollowingPartner(partner))
    {
        ClearFanClubPartnerState();
        return;
    }

    follower = GetFollowerObject();
    if (follower == NULL)
    {
        UpdateFollowingPokemon();
        follower = GetFollowerObject();
    }

    if (follower == NULL || !follower->active)
        return;

    packedPosition = VarGet(VAR_GY_FAN_CLUB_PARTNER_POS);
    if (packedPosition == 0)
    {
        NormalizeFanClubPartnerFollower(follower, FALSE);
        ClearFanClubPartnerState();
        return;
    }

    ClearObjectEventMovement(follower, &gSprites[follower->spriteId]);
    UnfreezeObjectEvent(follower);
    MoveObjectEventToMapCoords(follower, packedPosition & 0xFF, packedPosition >> 8);
    ParkFanClubPartnerFollower(follower);
    ObjectEventTurn(follower, DIR_EAST);
}

bool8 GoldenYellow_IsFanClubPartnerParked(struct Pokemon *partner)
{
    return IsInVermilionFanClub()
        && VarGet(VAR_GY_FAN_CLUB_PARTNER_STATE) == GY_FAN_CLUB_PARTNER_PARKED
        && IsCanonicalFollowingPartner(partner);
}

void GoldenYellow_CompleteFanClubPartnerOnFollower(void)
{
    struct ObjectEvent *follower = GetFollowerObject();

    // Emotion 30 is the single normal scene-end path. Normalize the complete
    // following-Pokemon movement state before clearing the authored scene flag.
    NormalizeFanClubPartnerFollower(follower, TRUE);
    ClearFanClubPartnerState();
}

void GoldenYellow_TryReleaseFanClubPartnerOnExit(struct ScriptContext *ctx)
{
    struct ObjectEvent *follower;

    (void)ctx;
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    // Yellow clears its Fan Club map-specific Pikachu state on returning to
    // Vermilion. Release slightly earlier, on the inward door row while the
    // player is walking south, so the GBA follower cannot cross the warp while
    // still owned by MOVEMENT_TYPE_NONE.
    if (!IsInVermilionFanClub()
     || VarGet(VAR_GY_FAN_CLUB_PARTNER_STATE) != GY_FAN_CLUB_PARTNER_PARKED
     || GetPlayerFacingDirection() != DIR_SOUTH)
        return;

    follower = GetFollowerObject();
    NormalizeFanClubPartnerFollower(follower, TRUE);
    ClearFanClubPartnerState();
}

void GoldenYellow_DebugPrepareBeforeFanClubCheckpoint(void)
{
    // The debug menu first reconstructs the existing post-Bill/Bulbasaur-ready
    // checkpoint. Redirect only its pending warp and P7C-specific state instead
    // of duplicating the accepted cumulative checkpoint logic.
    FlagClear(FLAG_GOT_BIKE_VOUCHER);
    ClearFanClubPartnerState();
    SetWarpDestination(MAP_GROUP(MAP_VERMILION_CITY),
                       MAP_NUM(MAP_VERMILION_CITY),
                       WARP_ID_NONE,
                       12,
                       18);
    ResetInitialPlayerAvatarState();
}
