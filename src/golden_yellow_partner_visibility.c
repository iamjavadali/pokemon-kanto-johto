#include "global.h"
#include "field_player_avatar.h"
#include "metatile_behavior.h"
#include "pokemon.h"
#include "constants/species.h"

// Deliberately avoid event_object_movement.h / follower_helper.h here. Their
// declaration marks the expansion implementation weak; this translation unit
// owns the strong Golden Yellow visibility policy.
struct Pokemon *GetPartnerAwareFollowingMon(void);

static bool32 IsCanonicalPartnerPikachuFollowing(void)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();

    return partner != NULL
        && GetMonData(partner, MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER;
}

bool32 IsFollowerVisible(void)
{
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    u8 hiddenAvatarFlags = FOLLOWER_INVISIBLE_FLAGS;
    bool32 hideForForcedMovementTile = MetatileBehavior_IsForcedMovementTile(player->currentMetatileBehavior);

    // Yellow's canonical Partner remains physically present through ordinary
    // traversal choreography. In particular, doors, stairs, entrances, exits,
    // and forced-movement warps must not trigger the expansion's Poké Ball
    // recall animation. Preserve the stock rules for surf/underwater/bikes and
    // preserve every visibility rule for ordinary following Pokémon.
    if (IsCanonicalPartnerPikachuFollowing())
    {
        hiddenAvatarFlags &= ~PLAYER_AVATAR_FLAG_FORCED_MOVE;
        hideForForcedMovementTile = FALSE;
    }

    return !(TestPlayerAvatarFlags(hiddenAvatarFlags)
          || MetatileBehavior_IsSurfableWaterOrUnderwater(player->previousMetatileBehavior)
          || hideForForcedMovementTile);
}
