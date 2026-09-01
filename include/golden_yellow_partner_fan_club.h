#ifndef GUARD_GOLDEN_YELLOW_PARTNER_FAN_CLUB_H
#define GUARD_GOLDEN_YELLOW_PARTNER_FAN_CLUB_H

#include "global.h"

struct Pokemon;
struct ScriptContext;

// P7C authored Vermilion Pokemon Fan Club lifecycle. The scene temporarily
// transfers the canonical Partner's existing following-Pokemon object from
// FOLLOW_PLAYER to a parked scene owner, then restores normal following after
// the player's authored A-button reaction.
void GoldenYellow_StartFanClubPartnerArrival(struct ScriptContext *ctx);
void GoldenYellow_RestoreFanClubPartnerOnFollower(struct ScriptContext *ctx);
void GoldenYellow_TryReleaseFanClubPartnerOnExit(struct ScriptContext *ctx);
bool8 GoldenYellow_IsFanClubPartnerParked(struct Pokemon *partner);
void GoldenYellow_CompleteFanClubPartnerOnFollower(void);

// Debug checkpoint adapter. Reuses the existing cumulative post-Bill checkpoint
// state and only redirects its pending warp to the pre-Fan-Club Vermilion spot.
void GoldenYellow_DebugPrepareBeforeFanClubCheckpoint(void);

#endif // GUARD_GOLDEN_YELLOW_PARTNER_FAN_CLUB_H
