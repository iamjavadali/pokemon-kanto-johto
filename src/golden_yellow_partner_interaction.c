#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "golden_yellow_partner_reaction.h"
#include "golden_yellow_partner_state.h"
#include "pokemon.h"
#include "script.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/species.h"

static bool8 GoldenYellow_WaitForPartnerPikachuFieldInteraction(void)
{
    return !GoldenYellow_IsPartnerPikachuReactionActive();
}

void GoldenYellow_TryPartnerPikachuFieldInteraction(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    struct ObjectEvent *follower = GetFollowerObject();

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    // VAR_RESULT tells EventScript_Follower whether this interaction belongs to
    // Yellow's canonical Partner path. Ordinary followers, including ordinary
    // SPECIES_PIKACHU, fall through to the expansion's existing behavior.
    gSpecialVar_Result = FALSE;

    if (partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER
     || follower == NULL
     || !follower->active)
        return;

    // Once the actual follower is confirmed as canonical Partner Pikachu, this
    // interaction is owned by Golden Yellow even if a transient resource/state
    // conflict prevents the reaction task from starting. Never fall through to
    // a contradictory generic follower message in that case.
    gSpecialVar_Result = TRUE;

    if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(GoldenYellow_SelectPartnerTalkReaction(partner)))
        return;

    // Keep the existing follower script locked until P3 has finished its full
    // cry/emote/movement/pose/portrait lifecycle. FIELD_TALK keeps P3's
    // automatic portrait flow while adding a short overworld-pose beat before
    // the portrait appears; the debug-only manual portrait gate is not used.
    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
}
