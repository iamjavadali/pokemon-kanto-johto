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

static bool8 GoldenYellow_WaitForPewterPartnerWake(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_CompletePewterPartnerWakeOnFollower();
    return TRUE;
}

void GoldenYellow_RestorePewterPartnerSleepNative(struct ScriptContext *ctx)
{
    (void)ctx;
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    GoldenYellow_RestorePewterPartnerSleepOnFollower();
}

void GoldenYellow_StartPewterPartnerSleepReaction(struct ScriptContext *ctx)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    gSpecialVar_Result = FALSE;

    // The saved sleep state is prepared immediately before this call. Commit the
    // authored scene only if the P3 reaction task successfully takes ownership;
    // otherwise normalize the follower and roll the scene state back to idle so
    // a later Jigglypuff interaction can safely retry.
    if (!GoldenYellow_StartPartnerPikachuReaction(GY_PARTNER_REACTION_SLEEPING))
    {
        GoldenYellow_CancelPewterPartnerSleepOnFollower();
        return;
    }

    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
    gSpecialVar_Result = TRUE;
}

static bool32 GoldenYellow_StartPewterPartnerWake(struct ScriptContext *ctx, struct Pokemon *partner)
{
    if (!GoldenYellow_IsPewterPartnerSleepActive(partner))
        return FALSE;

    if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(GY_PARTNER_REACTION_PEWTER_JIGGLYPUFF))
        return FALSE;

    // Direct A-button wake keeps the existing ACTIVE state until the reaction
    // finishes. Item/Poke Flute wake already enters WAKE_PENDING before this
    // function is reached through the map-resume path. Do not promote direct
    // field talk to WAKE_PENDING, otherwise the on-frame script becomes a second
    // owner of the same wake transition.
    SetupNativeScript(ctx, GoldenYellow_WaitForPewterPartnerWake);
    ctx->waitAfterCallNative = TRUE;
    return TRUE;
}

void GoldenYellow_ResumePendingPewterPartnerWake(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (!GoldenYellow_IsPewterPartnerWakePending())
        return;

    if (partner == NULL || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER)
    {
        GoldenYellow_CancelPewterPartnerSleepOnFollower();
        return;
    }

    GoldenYellow_StartPewterPartnerWake(ctx, partner);
}

void GoldenYellow_TryPartnerPikachuFieldInteraction(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    struct ObjectEvent *follower = GetFollowerObject();
    u8 reaction;
    bool8 hasOneShotReaction;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    gSpecialVar_Result = FALSE;

    if (partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER
     || follower == NULL
     || !follower->active)
        return;

    gSpecialVar_Result = TRUE;

    if (GoldenYellow_IsPewterPartnerSleepActive(partner))
    {
        GoldenYellow_StartPewterPartnerWake(ctx, partner);
        return;
    }

    hasOneShotReaction = GoldenYellow_TryGetPartnerPikachuOneShotReaction(partner, &reaction);
    if (!hasOneShotReaction)
        reaction = GoldenYellow_SelectPartnerTalkReaction(partner);

    if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
        return;

    if (hasOneShotReaction)
        GoldenYellow_ConsumePartnerPikachuOneShotReaction(partner);

    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
    ctx->waitAfterCallNative = TRUE;
}
