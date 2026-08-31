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
    u8 reaction;
    bool8 hasOneShotReaction;

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

    // P6 one-shot gameplay modifiers sit immediately above P5's normal
    // friendship/mood matrix. P7 authored/status/area reactions can layer above
    // this resolver later without changing either subsystem.
    hasOneShotReaction = GoldenYellow_TryGetPartnerPikachuOneShotReaction(partner, &reaction);
    if (!hasOneShotReaction)
        reaction = GoldenYellow_SelectPartnerTalkReaction(partner);

    if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
        return;

    // Consume only after the reaction task owns the interaction. If a transient
    // task/resource conflict prevents start-up, the source one-shot remains
    // pending for the next eligible Partner talk.
    if (hasOneShotReaction)
        GoldenYellow_ConsumePartnerPikachuOneShotReaction(partner);

    // Keep the existing follower script locked until P3 has finished its full
    // cry/emote/movement/pose/portrait lifecycle. FIELD_TALK keeps P3's
    // automatic portrait flow while adding a short overworld-pose beat before
    // the portrait appears; the debug-only manual portrait gate is not used.
    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
}
