#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "golden_yellow_partner_fan_club.h"
#include "golden_yellow_partner_reaction.h"
#include "golden_yellow_partner_state.h"
#include "pokemon.h"
#include "script.h"
#include "constants/event_objects.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/maps.h"
#include "constants/species.h"

static bool8 GoldenYellow_WaitForPartnerPikachuFieldInteraction(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_ClearPartnerPikachuReactionObject();
    return TRUE;
}

static bool8 GoldenYellow_WaitForPewterPartnerWake(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_CompletePewterPartnerWakeOnFollower();
    return TRUE;
}

static bool8 GoldenYellow_WaitForFanClubPartnerRejoin(void)
{
    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_CompleteFanClubPartnerOnFollower();
    return TRUE;
}

static bool32 GoldenYellow_IsBillPartnerInteractionObject(void)
{
    struct ObjectEvent *selectedObject;

    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_ROUTE25_SEA_COTTAGE)
     || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_ROUTE25_SEA_COTTAGE)
     || gSelectedObjectEvent >= OBJECT_EVENTS_COUNT)
        return FALSE;

    selectedObject = &gObjectEvents[gSelectedObjectEvent];

    // The map/location and selected-object test identify Bill's dedicated
    // scene object. Canonical Partner identity is still established separately
    // through GetPartnerAwareFollowingMon() + SPECIES_PIKACHU_STARTER below;
    // graphics alone never qualifies an ordinary Pikachu as the Partner.
    return selectedObject->active
        && selectedObject->graphicsId == OBJ_EVENT_GFX_PIKACHU_FRLG;
}

static bool32 GoldenYellow_IsPokemonTowerPartnerInteractionMap(void)
{
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_POKEMON_TOWER_1F))
        return FALSE;

    // Yellow applies the authored afraid reaction to direct Partner talk on
    // every Pokemon Tower floor. List each FRLG map explicitly instead of
    // depending on map-number contiguity so later map-table edits cannot widen
    // or silently break this story-specific precedence rule.
    switch (gSaveBlock1Ptr->location.mapNum)
    {
    case MAP_NUM(MAP_POKEMON_TOWER_1F):
    case MAP_NUM(MAP_POKEMON_TOWER_2F):
    case MAP_NUM(MAP_POKEMON_TOWER_3F):
    case MAP_NUM(MAP_POKEMON_TOWER_4F):
    case MAP_NUM(MAP_POKEMON_TOWER_5F):
    case MAP_NUM(MAP_POKEMON_TOWER_6F):
    case MAP_NUM(MAP_POKEMON_TOWER_7F):
        return TRUE;
    default:
        return FALSE;
    }
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
    struct ObjectEvent *follower;
    u8 reaction;
    bool8 hasOneShotReaction;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    gSpecialVar_Result = FALSE;
    GoldenYellow_ClearPartnerPikachuReactionObject();

    if (partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER)
        return;

    // P7B: Yellow's Bill-house direct-talk selector is an authored map
    // precedence override, not a rewrite of the already accepted Bill scene.
    // Yellow SCRIPT0/Emotion23 and SCRIPT5/Emotion27 remain owned by the
    // automatic transformed/restored cutscene. Manual A-button interaction is:
    //   before EVENT_MET_BILL_2 equivalent -> Emotion32
    //   after  EVENT_MET_BILL_2 equivalent -> Emotion31
    // Golden Yellow's persistent equivalent is FLAG_HELPED_BILL_IN_SEA_COTTAGE.
    if (GoldenYellow_IsBillPartnerInteractionObject())
    {
        gSpecialVar_Result = TRUE;
        reaction = FlagGet(FLAG_HELPED_BILL_IN_SEA_COTTAGE)
                 ? GY_PARTNER_REACTION_BILL_POST_STATE
                 : GY_PARTNER_REACTION_BILL_INTERMEDIATE;

        GoldenYellow_SetPartnerPikachuReactionObject(gSelectedObjectEvent);
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(reaction))
        {
            GoldenYellow_ClearPartnerPikachuReactionObject();
            return;
        }

        SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
        ctx->waitAfterCallNative = TRUE;
        return;
    }

    follower = GetFollowerObject();
    if (follower == NULL || !follower->active)
        return;

    gSpecialVar_Result = TRUE;

    // P7C: while Partner is parked beside the Fan Club Pikachu, direct talk has
    // authored precedence over P7A/P6/P5. Yellow Emotion30 is the manual release
    // reaction; only after it finishes does the proven P7A-style lifecycle hand
    // the same object back to FOLLOW_PLAYER.
    if (GoldenYellow_IsFanClubPartnerParked(partner))
    {
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(GY_PARTNER_REACTION_FAN_CLUB_MAX_AFFECTION))
            return;

        SetupNativeScript(ctx, GoldenYellow_WaitForFanClubPartnerRejoin);
        ctx->waitAfterCallNative = TRUE;
        return;
    }

    if (GoldenYellow_IsPewterPartnerSleepActive(partner))
    {
        GoldenYellow_StartPewterPartnerWake(ctx, partner);
        return;
    }

    // P7D: Yellow gives direct Partner talk on Pokemon Tower 1F-7F a repeatable
    // map-specific override. Emotion22 outranks P6 one-shot modifiers and P5
    // friendship/mood selection, but it owns no scene state and consumes no
    // modifier; leaving the Tower naturally returns to normal interaction.
    if (GoldenYellow_IsPokemonTowerPartnerInteractionMap())
    {
        if (!GoldenYellow_StartPartnerPikachuFieldTalkReaction(GY_PARTNER_REACTION_TOWER_AFRAID))
            return;

        SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuFieldInteraction);
        ctx->waitAfterCallNative = TRUE;
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
