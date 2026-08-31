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

enum GoldenYellowPewterPartnerSleepStateFix1
{
    GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX1 = 0,
    GY_PEWTER_PARTNER_SLEEP_ACTIVE_FIX1 = 1,
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
    VarSet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE, GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX1);
    VarSet(VAR_GY_PEWTER_PARTNER_SLEEP_POS, 0);
}

static void ParkPewterPartnerFollower(struct ObjectEvent *follower)
{
    if (follower == NULL || !follower->active)
        return;

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

    // Partner Pikachu is an OW following-Pokemon object, not an NPC follower.
    // Require the canonical starter and its actual OBJ_EVENT_ID_FOLLOWER object.
    if (!IsInPewterPokemonCenter1F()
     || VarGet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE) != GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX1
     || !IsCanonicalFollowingPartner(partner)
     || GetMonData(partner, MON_DATA_HP) == 0
     || GetMonData(partner, MON_DATA_STATUS) != 0
     || follower == NULL
     || !follower->active)
        return;

    StorePewterPartnerSleepPosition(follower);
    VarSet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE, GY_PEWTER_PARTNER_SLEEP_ACTIVE_FIX1);

    // Keep the real follower object in place. MOVEMENT_TYPE_NONE prevents the
    // normal follow callback and restores ordinary object collision while held
    // script movements (the authored sleepy animation) remain available.
    SetTrainerMovementType(follower, MOVEMENT_TYPE_NONE);
    gSpecialVar_Result = TRUE;
}

void GoldenYellow_RestorePewterPartnerSleepOnFollower(void)
{
    struct Pokemon *partner;
    struct ObjectEvent *follower;
    u16 packedPosition;
    s16 x;
    s16 y;

    if (VarGet(VAR_GY_PEWTER_PARTNER_SLEEP_STATE) == GY_PEWTER_PARTNER_SLEEP_INACTIVE_FIX1)
        return;

    partner = GetPartnerAwareFollowingMon();
    if (!IsInPewterPokemonCenter1F() || !IsCanonicalFollowingPartner(partner))
    {
        ClearPewterPartnerSleepState();
        return;
    }

    // Recreate the normal following-Pokemon object if field loading has not
    // already done so, then convert that same object back into its parked state.
    follower = GetFollowerObject();
    if (follower == NULL)
    {
        UpdateFollowingPokemon();
        follower = GetFollowerObject();
    }

    // Preserve the saved scene state if the field engine is temporarily hiding
    // the follower during load; a later restore can safely retry.
    if (follower == NULL || !follower->active)
        return;

    packedPosition = VarGet(VAR_GY_PEWTER_PARTNER_SLEEP_POS);
    if (packedPosition == 0)
    {
        ClearPewterPartnerSleepState();
        return;
    }

    x = packedPosition & 0xFF;
    y = packedPosition >> 8;

    ObjectEventClearHeldMovementIfActive(follower);
    MoveObjectEventToMapCoords(follower, x, y);
    ObjectEventTurn(follower, DIR_SOUTH);
    ParkPewterPartnerFollower(follower);
}

void GoldenYellow_CompletePewterPartnerWakeOnFollower(void)
{
    struct ObjectEvent *follower = GetFollowerObject();

    if (follower != NULL && follower->active)
    {
        struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
        s16 deltaX = player->currentCoords.x - follower->currentCoords.x;
        s16 deltaY = player->currentCoords.y - follower->currentCoords.y;

        if (deltaX < 0)
            deltaX = -deltaX;
        if (deltaY < 0)
            deltaY = -deltaY;

        ObjectEventClearHeldMovementIfActive(follower);

        // Item/Poke Flute wake can occur across the room. Re-anchor the same
        // following-Pokemon object behind the player before restoring follow.
        if (deltaX + deltaY > 1)
            MoveObjectEventToMapCoords(follower, player->previousCoords.x, player->previousCoords.y);

        SetTrainerMovementType(follower, MOVEMENT_TYPE_FOLLOW_PLAYER);
        follower->invisible = FALSE;
        gSprites[follower->spriteId].invisible = FALSE;
    }

    ClearPewterPartnerSleepState();
}
