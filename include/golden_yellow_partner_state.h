#ifndef GUARD_GOLDEN_YELLOW_PARTNER_STATE_H
#define GUARD_GOLDEN_YELLOW_PARTNER_STATE_H

#include "global.h"
#include "pokemon.h"

// P5 keeps pokeemerald-expansion's MON_DATA_FRIENDSHIP as the single long-term
// relationship value and layers Yellow's short-term Partner mood on top.
u8 GoldenYellow_GetPartnerPikachuMood(struct Pokemon *partner);
u8 GoldenYellow_SelectPartnerTalkReaction(struct Pokemon *partner);
u8 GoldenYellow_SelectPartnerTalkReactionForState(u8 friendship, u8 mood);

// Deterministic validation helper for P5's 7x5 Yellow reaction matrix. This is
// intentionally not wired into normal gameplay or the story checkpoint system.
void GoldenYellow_DebugSetPartnerPikachuState(struct Pokemon *partner, u8 friendship, u8 mood);

// Private base symbols emitted by src/pokemon.c. The aliases below are enabled
// only from that translation unit so Golden Yellow can extend the expansion's
// existing friendship/item/battle seams rather than replacing their internals.
void GoldenYellow_AdjustFriendshipBase(struct Pokemon *mon, u8 event);
bool8 GoldenYellow_ExecuteTableBasedItemEffectBase(struct Pokemon *mon, enum Item item, u8 partyIndex, u8 moveIndex);
void GoldenYellow_ZeroEnemyPartyMonsBase(void);

#ifdef GOLDEN_YELLOW_PARTNER_POKEMON_IMPLEMENTATION
#define AdjustFriendship              GoldenYellow_AdjustFriendshipBase
#define ExecuteTableBasedItemEffect   GoldenYellow_ExecuteTableBasedItemEffectBase
#define ZeroEnemyPartyMons            GoldenYellow_ZeroEnemyPartyMonsBase
#endif

#endif // GUARD_GOLDEN_YELLOW_PARTNER_STATE_H
