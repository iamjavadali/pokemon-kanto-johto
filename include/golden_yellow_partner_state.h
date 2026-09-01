#ifndef GUARD_GOLDEN_YELLOW_PARTNER_STATE_H
#define GUARD_GOLDEN_YELLOW_PARTNER_STATE_H

#include "global.h"
#include "pokemon.h"
#include "constants/event_objects.h"

struct ObjectEvent;
struct ScriptContext;

// Shared canonical identity predicate for the Yellow starter's active
// following-Pokemon object. Traversal code uses this instead of treating every
// ordinary Pikachu or generic follower as the authored Partner.
bool8 GoldenYellow_IsCanonicalPartnerPikachuFollower(const struct ObjectEvent *followerObject);

// P5 keeps pokeemerald-expansion's MON_DATA_FRIENDSHIP as the single long-term
// relationship value and layers Yellow's short-term Partner mood on top.
u8 GoldenYellow_GetPartnerPikachuMood(struct Pokemon *partner);
u8 GoldenYellow_SelectPartnerTalkReaction(struct Pokemon *partner);
u8 GoldenYellow_SelectPartnerTalkReactionForState(u8 friendship, u8 mood);

// P6 stores Yellow's one-byte transient emotion modifier separately from mood.
// The modifier is only consumed after its field-talk reaction successfully starts.
bool8 GoldenYellow_TryGetPartnerPikachuOneShotReaction(struct Pokemon *partner, u8 *reaction);
void GoldenYellow_ConsumePartnerPikachuOneShotReaction(struct Pokemon *partner);

// P7A models the Pewter Jigglypuff sequence as a saved authored overworld state.
// It never writes battle sleep into MON_DATA_STATUS: UI and sleep-curing items
// resolve through the scene state while the canonical Partner remains parked.
bool8 GoldenYellow_IsPewterPartnerSleepActive(struct Pokemon *partner);
bool8 GoldenYellow_IsPewterPartnerWakePending(void);
void GoldenYellow_TryStartPewterPartnerSleep(struct ScriptContext *ctx);
void GoldenYellow_RestorePewterPartnerSleep(void);
void GoldenYellow_RequestPewterPartnerWake(void);
void GoldenYellow_CompletePewterPartnerWake(void);

// P7A following-Pokemon lifecycle entry points. Partner Pikachu remains the
// expansion's OBJ_EVENT_ID_FOLLOWER object; these helpers temporarily park that
// object for the authored sleep scene and always normalize it before returning
// control to MOVEMENT_TYPE_FOLLOW_PLAYER.
void GoldenYellow_TryStartPewterPartnerSleepOnFollower(struct ScriptContext *ctx);
void GoldenYellow_RestorePewterPartnerSleepOnFollower(void);
void GoldenYellow_CancelPewterPartnerSleepOnFollower(void);
void GoldenYellow_CompletePewterPartnerWakeOnFollower(void);

// Deterministic validation helper for P5's 7x5 Yellow reaction matrix. This is
// intentionally not wired into normal gameplay or the story checkpoint system.
void GoldenYellow_DebugSetPartnerPikachuState(struct Pokemon *partner, u8 friendship, u8 mood);

// Private base symbols emitted by src/pokemon.c. The aliases below are enabled
// only from that translation unit so Golden Yellow can extend the expansion's
// existing friendship/item/battle/move seams rather than replacing internals.
void GoldenYellow_AdjustFriendshipBase(struct Pokemon *mon, u8 event);
bool8 GoldenYellow_ExecuteTableBasedItemEffectBase(struct Pokemon *mon, enum Item item, u8 partyIndex, u8 moveIndex);
void GoldenYellow_ZeroEnemyPartyMonsBase(void);
u16 GoldenYellow_GiveMoveToMonBase(struct Pokemon *mon, enum Move move);
u16 GoldenYellow_GiveMoveToBoxMonBase(struct BoxPokemon *boxMon, enum Move move);
enum Move GoldenYellow_MonTryLearningNewMoveAtLevelBase(struct Pokemon *mon, bool32 firstMove, u32 level);
enum Move GoldenYellow_MonTryLearningNewMoveBase(struct Pokemon *mon, bool8 firstMove);
void GoldenYellow_SetMonMoveSlotBase(struct Pokemon *mon, enum Move move, u8 slot);
void GoldenYellow_SetBoxMonDataBase(struct BoxPokemon *boxMon, s32 field, const void *dataArg);

#ifdef GOLDEN_YELLOW_PARTNER_POKEMON_IMPLEMENTATION
#define AdjustFriendship                 GoldenYellow_AdjustFriendshipBase
#define ExecuteTableBasedItemEffect      GoldenYellow_ExecuteTableBasedItemEffectBase
#define ZeroEnemyPartyMons               GoldenYellow_ZeroEnemyPartyMonsBase
#define GiveMoveToMon                     GoldenYellow_GiveMoveToMonBase
#define GiveMoveToBoxMon                  GoldenYellow_GiveMoveToBoxMonBase
#define MonTryLearningNewMoveAtLevel      GoldenYellow_MonTryLearningNewMoveAtLevelBase
#define MonTryLearningNewMove             GoldenYellow_MonTryLearningNewMoveBase
#define SetMonMoveSlot                    GoldenYellow_SetMonMoveSlotBase
#define SetBoxMonData                     GoldenYellow_SetBoxMonDataBase
#endif

#endif // GUARD_GOLDEN_YELLOW_PARTNER_STATE_H
