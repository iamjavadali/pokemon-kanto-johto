#include "global.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "golden_yellow_partner_state.h"

// Avoid including follower_npc.h here: that header deliberately marks the two
// expansion fallbacks weak so these P7A policy overrides remain strong symbols.
bool32 PlayerHasFollowerNPC(void);
u32 GetFollowerNPCObjectId(void);

// follower_npc.c keeps the expansion's original implementation as a weak
// fallback. P7A overrides only the direction resolver so the canonical Partner
// can remain a real, interactable follower object while the Pewter authored
// sleep state suspends automatic following.
enum Direction DetermineFollowerNPCDirection(struct ObjectEvent *player, struct ObjectEvent *follower)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();

    if (GoldenYellow_IsPewterPartnerSleepActive(partner))
        return DIR_NONE;

    if (player->currentCoords.x == follower->currentCoords.x
     && player->currentCoords.y == follower->currentCoords.y)
        return DIR_NONE;

    return DetermineObjectEventDirectionFromObject(player, follower);
}

// Normal followers are collision-exempt so they cannot trap the player. During
// the authored sleep scene Pikachu is intentionally parked, so restore ordinary
// NPC solidity until the wake sequence completes.
bool32 FollowerNPC_IsCollisionExempt(struct ObjectEvent *obstacle, struct ObjectEvent *collider)
{
    struct ObjectEvent *follower;
    struct ObjectEvent *player;
    struct Pokemon *partner;

    if (!PlayerHasFollowerNPC())
        return FALSE;

    follower = &gObjectEvents[GetFollowerNPCObjectId()];
    player = &gObjectEvents[gPlayerAvatar.objectEventId];
    partner = GetPartnerAwareFollowingMon();

    if (obstacle == follower && collider == player)
    {
        if (GoldenYellow_IsPewterPartnerSleepActive(partner))
            return FALSE;
        return TRUE;
    }

    return FALSE;
}
