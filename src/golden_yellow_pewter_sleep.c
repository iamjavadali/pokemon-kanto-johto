#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "pokemon.h"
#include "script.h"
#include "sprite.h"
#include "constants/maps.h"
#include "constants/species.h"

#define VAR_GY_PEWTER_PARTNER_SLEEP_STATE VAR_UNUSED_0x40FB
#define VAR_GY_PEWTER_PARTNER_SLEEP_POS   VAR_UNUSED_0x40FC

enum GoldenYellowPewterPartnerSleepStateFix2
{
    GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX2 = 0,
    GY_PEWTER_PARTNER_SLEEP_ACTIVE_FIX2 = 1,
};

static bool32 IsInPewterPokemonCenter1F(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_PEWTER_CITY_POKEMON_CENTER_1F)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_PEWTER_CITY_POKEMON_CENTER_1F);
}

static bool32 IsCanonicalFollowingPartner(struct Pokemon *partner)
{
    return partner != NULL
        && GetMonData(partner, MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER;
}

static void StorePewterPartnerSleepPosition(const struct ObjectEvent *follower)
{
    u16 packedPosition;

    if (follower == NULL)
        return;

    packedPosition = (u8)follower->currentCoords.x
                   | ((u16)(u8)follower->currentCoords.y << 8);
    VarSet(VAR_GY_PEWTER_PARTNER_SLEEP_POS, packedPosition);
}

static void ClearPewterPartnerSleepState(void)
{
    VarSet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE, GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX2);
    VarSet(VAR_GY_PEWTER_PARTNER_SLEEP_POS, 0);
}

static void NormalizePewterPartnerFollower(struct ObjectEvent *follower, bool32 reanchorIfDistant)
{
    if (follower == NULL || !follower->active)
        return;

    ObjectEventClearHeldMovementIfActive(follower);
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

static void ParkPewterPartnerFollower(struct ObjectEvent *follower)
{
    if (follower == NULL || !follower->active)
        return;

    // Match the previously proven P7A sleep-start behavior. Clear only an
    // ordinary held follow step, then park the existing follower object. Do not
    // unfreeze/reset it through FOLLOW_PLAYER immediately before P3 takes over:
    // the reaction director owns the subsequent ScriptMovement choreography.
    ObjectEventClearHeldMovementIfActive(follower);
    SetTrainerMovementType(follower, MOVEMENT_TYPE_NONE);
    follower->invisible = FALSE;
    gSprites[follower->spriteId].invisible = FALSE;
}

void GoldenYellow_TryStartPewterPartnerSleepOnFollower(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    struct ObjectEvent *follower = GetFollowerObject();

    (void)ctx;
    gSpecialVar_Result = FALSE;

    if (!IsInPewterPokemonCenter1F()
     || VarGet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE) != GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX2
     || !IsCanonicalFollowingPartner(partner)
     || GetMonData(partner, MON_DATA_HP) == 0
     || GetMonData(partner, MON_DATA_STATUS) != 0
     || follower == NULL
     || !follower->active)
        return;

    // Preserve the proven startup order: save the current follower position,
    // commit the authored sleep state, then park that same follower object.
    // Transactional rollback remains in GoldenYellow_StartPewterPartnerSleepReaction
    // if the P3 reaction task cannot take ownership.
    StorePewterPartnerSleepPosition(follower);
    VarSet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE, GY_PEWTER_PARTNER_SLEEP_ACTIVE_FIX2);
    ParkPewterPartnerFollower(follower);
    gSpecialVar_Result = TRUE;
}

void GoldenYellow_RestorePewterPartnerSleepOnFollower(void)
{
    struct Pokemon *partner;
    struct ObjectEvent *follower;
    u16 packedPosition;
    s16 x;
    s16 y;

    if (VarGet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE) == GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX2)
        return;

    partner = GetPartnerAwareFollowingMon();
    if (!IsInPewterPokemonCenter1F() || !IsCanonicalFollowingPartner(partner))
    {
        ClearPewterPartnerSleepState();
        return;
    }

    follower = GetFollowerObject();
    if (follower == NULL)
    {
        UpdateFollowingPokemon();
        follower = GetFollowerObject();
    }

    // Field loading can transiently hide the follower. Preserve the saved state
    // and allow the next transition/resume pass to retry instead of corrupting
    // the scene.
    if (follower == NULL || !follower->active)
        return;

    packedPosition = VarGet(VAR_GY_PEWTER_PARTNER_SLEEP_POS);
    if (packedPosition == 0)
    {
        NormalizePewterPartnerFollower(follower, FALSE);
        ClearPewterPartnerSleepState();
        return;
    }

    x = packedPosition & 0xFF;
    y = packedPosition >> 8;

    ObjectEventClearHeldMovementIfActive(follower);
    UnfreezeObjectEvent(follower);
    MoveObjectEventToMapCoords(follower, x, y);
    ObjectEventTurn(follower, DIR_SOUTH);
    ParkPewterPartnerFollower(follower);
}

void GoldenYellow_CancelPewterPartnerSleepOnFollower(void)
{
    struct ObjectEvent *follower = GetFollowerObject();

    // Idempotent rollback/cleanup. It is safe to call after a failed reaction
    // start, invalid pending wake, or a partially completed scene.
    NormalizePewterPartnerFollower(follower, FALSE);
    ClearPewterPartnerSleepState();
}

void GoldenYellow_CompletePewterPartnerWakeOnFollower(void)
{
    struct ObjectEvent *follower = GetFollowerObject();

    // Wake completion is the single normal scene-end path. Normalize every
    // follower invariant before clearing persistent state so the next Jigglypuff
    // interaction begins from a clean FOLLOW_PLAYER object.
    NormalizePewterPartnerFollower(follower, TRUE);
    ClearPewterPartnerSleepState();
}
