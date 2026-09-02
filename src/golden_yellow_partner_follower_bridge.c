#define GOLDEN_YELLOW_PARTNER_P8_BRIDGE_IMPLEMENTATION

#include "global.h"
#include "battle_util.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_effect.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "follower_helper.h"
#include "golden_yellow_partner_fan_club.h"
#include "golden_yellow_partner_reaction.h"
#include "golden_yellow_partner_state.h"
#include "pokemon.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "constants/abilities.h"
#include "constants/battle.h"
#include "constants/event_objects.h"
#include "constants/field_effects.h"
#include "constants/maps.h"
#include "constants/pokemon.h"
#include "constants/region_map_sections.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/weather.h"

// P8 intentionally keeps the expansion's full GetFollowerAction untouched for
// ordinary followers. This file supplies a context-only sibling for canonical
// Partner Pikachu and wraps the accepted P7 interaction seam.

struct GoldenYellowFollowerSpecialEmote
{
    u16 index;
    u8 emotion;
};

static EWRAM_DATA struct ScriptContext *sPartnerContextScript = NULL;

static void GoldenYellow_ObjectEventEmote(struct ObjectEvent *objEvent, u8 emotion)
{
    emotion %= FOLLOWER_EMOTION_LENGTH;
    ObjectEventGetLocalIdAndMap(objEvent, &gFieldEffectArguments[0], &gFieldEffectArguments[1], &gFieldEffectArguments[2]);
    gFieldEffectArguments[7] = emotion;
    FieldEffectStart(FLDEFF_EMOTE);
}

static void GoldenYellow_DispatchFollowerConditionalMessage(struct ScriptContext *ctx, u32 index)
{
    u32 i;
    u32 emotion = gFollowerConditionalMessages[index].emotion;

    GoldenYellow_ObjectEventEmote(GetFollowerObject(), emotion);
    ctx->data[0] = (u32)gFollowerConditionalMessages[index].text;

    if (gFollowerConditionalMessages[index].textSpread)
    {
        for (i = 0; i < 4; i++)
        {
            if (!((u32 *)gFollowerConditionalMessages[index].text)[i])
                break;
        }
        ctx->data[0] = i ? ((u32 *)gFollowerConditionalMessages[index].text)[Random() % i] : 0;
    }

    ScriptCall(ctx, gFollowerConditionalMessages[index].script
                  ? gFollowerConditionalMessages[index].script
                  : gFollowerBasicMessages[emotion].script);
}

static void GoldenYellow_DispatchFollowerCCondition(struct ScriptContext *ctx, u32 emotion, u32 index)
{
    GoldenYellow_ObjectEventEmote(GetFollowerObject(), emotion);
    ctx->data[0] = (u32)gFollowerBasicMessages[emotion].messages[index].text;
    ScriptCall(ctx, gFollowerBasicMessages[emotion].messages[index].script
                  ? gFollowerBasicMessages[emotion].messages[index].script
                  : gFollowerBasicMessages[emotion].script);
}

void GetFollowerContextAction(struct ScriptContext *ctx)
{
    u32 species;
    s32 multi;
    struct GoldenYellowFollowerSpecialEmote condEmotes[16] = {0};
    u32 condCount = 0;
    u32 emotion;
    struct ObjectEvent *objEvent = GetFollowerObject();
    struct Pokemon *mon = GetPartnerAwareFollowingMon();
    u8 emotionWeight[FOLLOWER_EMOTION_LENGTH] =
    {
        [FOLLOWER_EMOTION_HAPPY] = 10,
        [FOLLOWER_EMOTION_NEUTRAL] = 15,
        [FOLLOWER_EMOTION_SAD] = 5,
        [FOLLOWER_EMOTION_UPSET] = 15,
        [FOLLOWER_EMOTION_ANGRY] = 15,
        [FOLLOWER_EMOTION_PENSIVE] = 15,
        [FOLLOWER_EMOTION_LOVE] = 0,
        [FOLLOWER_EMOTION_SURPRISE] = 10,
        [FOLLOWER_EMOTION_CURIOUS] = 10,
        [FOLLOWER_EMOTION_MUSIC] = 15,
        [FOLLOWER_EMOTION_POISONED] = 0,
    };
    u32 i, j;
    bool32 pickedCCondition = FALSE;
    bool32 pickedTableCondition = FALSE;

    // Match GetFollowerAction's dynamic-script convention. If neither
    // conditional layer wins, this becomes a silent return to the common end.
    ScriptJump(ctx, EventScript_FollowerEnd);

    if (mon == NULL || objEvent == NULL || !objEvent->active)
        return;

    species = GetMonData(mon, MON_DATA_SPECIES);
    multi = GetMonData(mon, MON_DATA_FRIENDSHIP);
    if (multi > 80)
    {
        emotionWeight[FOLLOWER_EMOTION_HAPPY] = 20;
        emotionWeight[FOLLOWER_EMOTION_UPSET] = 5;
        emotionWeight[FOLLOWER_EMOTION_ANGRY] = 5;
        emotionWeight[FOLLOWER_EMOTION_LOVE] = 20;
        emotionWeight[FOLLOWER_EMOTION_MUSIC] = 20;
    }
    if (multi > 170)
    {
        emotionWeight[FOLLOWER_EMOTION_HAPPY] = 30;
        emotionWeight[FOLLOWER_EMOTION_LOVE] = 30;
    }

    // Preserve GetFollowerAction's C-based contextual candidates and emotion
    // weighting. Their 50% consideration gate is retained below.
    if (GetCurrentWeather() == WEATHER_SUNNY_CLOUDS)
        condEmotes[condCount++] = (struct GoldenYellowFollowerSpecialEmote){.emotion = FOLLOWER_EMOTION_HAPPY, .index = 31};

    multi = SAFE_DIV(mon->hp * 100, mon->maxHP);
    if (multi < 20)
    {
        emotionWeight[FOLLOWER_EMOTION_SAD] = 30;
        condEmotes[condCount++] = (struct GoldenYellowFollowerSpecialEmote){.emotion = FOLLOWER_EMOTION_SAD, .index = 4};
        condEmotes[condCount++] = (struct GoldenYellowFollowerSpecialEmote){.emotion = FOLLOWER_EMOTION_SAD, .index = 5};
    }
    if (multi < 50 || (mon->status & STATUS1_PARALYSIS))
    {
        emotionWeight[FOLLOWER_EMOTION_SAD] = 30;
        condEmotes[condCount++] = (struct GoldenYellowFollowerSpecialEmote){.emotion = FOLLOWER_EMOTION_SAD, .index = 6};
    }

    if (GetCurrentMapMusic() == MUS_GYM || GetCurrentMapMusic() == MUS_RG_GYM)
    {
        switch (gMapHeader.regionMapSectionId)
        {
        case MAPSEC_RUSTBORO_CITY:
        case MAPSEC_PEWTER_CITY:
            multi = TYPE_ROCK;
            break;
        case MAPSEC_DEWFORD_TOWN:
            multi = TYPE_FIGHTING;
            break;
        case MAPSEC_MAUVILLE_CITY:
        case MAPSEC_VERMILION_CITY:
            multi = TYPE_ELECTRIC;
            break;
        case MAPSEC_LAVARIDGE_TOWN:
        case MAPSEC_CINNABAR_ISLAND:
            multi = TYPE_FIRE;
            break;
        case MAPSEC_PETALBURG_CITY:
            multi = TYPE_NORMAL;
            break;
        case MAPSEC_FORTREE_CITY:
            multi = TYPE_FLYING;
            break;
        case MAPSEC_MOSSDEEP_CITY:
        case MAPSEC_SAFFRON_CITY:
            multi = TYPE_PSYCHIC;
            break;
        case MAPSEC_SOOTOPOLIS_CITY:
        case MAPSEC_CERULEAN_CITY:
            multi = TYPE_WATER;
            break;
        case MAPSEC_CELADON_CITY:
            multi = TYPE_GRASS;
            break;
        case MAPSEC_FUCHSIA_CITY:
            multi = TYPE_POISON;
            break;
        case MAPSEC_VIRIDIAN_CITY:
            multi = TYPE_GROUND;
            break;
        default:
            multi = NUMBER_OF_MON_TYPES;
        }

        if (multi < NUMBER_OF_MON_TYPES)
        {
            multi = GetOverworldTypeEffectiveness(mon, multi);
            if (multi <= UQ_4_12(0.5))
                condEmotes[condCount++] = (struct GoldenYellowFollowerSpecialEmote){.emotion = FOLLOWER_EMOTION_HAPPY, .index = 32};
            else if (multi >= UQ_4_12(2.0))
                condEmotes[condCount++] = (struct GoldenYellowFollowerSpecialEmote){.emotion = FOLLOWER_EMOTION_SAD, .index = 7};
        }
    }

    emotion = RandomWeightedIndex(emotionWeight, FOLLOWER_EMOTION_LENGTH);
    if ((mon->status & STATUS1_PSN_ANY) && GetMonAbility(mon) != ABILITY_POISON_HEAL)
        emotion = FOLLOWER_EMOTION_POISONED;

    // Keep the basic-pool roll even in context-only mode so the random-call
    // sequence before the two conditional selectors matches GetFollowerAction.
    multi = Random() % gFollowerBasicMessages[emotion].length;

    // Existing 50% C-condition gate plus reservoir sampling. Unlike the stock
    // selector, preserve provenance so a surviving basic roll can be rejected.
    for (i = (Random() & 1) ? condCount : 0, j = 1; i < condCount; i++)
    {
        if (condEmotes[i].emotion == emotion && (Random() < 0x10000 / (j++)))
        {
            multi = condEmotes[i].index;
            pickedCCondition = TRUE;
        }
    }

    // Existing independent 50% table-condition gate and weighted reservoir
    // sampling. A table winner retains precedence over a C-based winner.
    for (i = (Random() & 1) ? COND_MSG_COUNT : 0, j = 1; i < COND_MSG_COUNT; i++)
    {
        const struct FollowerMsgInfoExtended *info = &gFollowerConditionalMessages[i];

        if (!CheckMsgInfo(info, mon, species, objEvent))
            continue;

        if (Random() < (0x10000 / (j++)) * (info->weight ? info->weight : 1))
        {
            multi = i;
            pickedTableCondition = TRUE;
        }
    }

    if (pickedTableCondition)
    {
        GoldenYellow_DispatchFollowerConditionalMessage(ctx, multi);
        return;
    }

    if (pickedCCondition)
        GoldenYellow_DispatchFollowerCCondition(ctx, emotion, multi);
}

static bool32 GoldenYellow_IsBillPartnerInteractionObjectP8(void)
{
    struct ObjectEvent *selectedObject;
    struct ObjectEvent *follower;

    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_ROUTE25_SEA_COTTAGE)
     || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_ROUTE25_SEA_COTTAGE)
     || gSelectedObjectEvent >= OBJECT_EVENTS_COUNT)
        return FALSE;

    selectedObject = &gObjectEvents[gSelectedObjectEvent];
    follower = GetFollowerObject();

    return follower != NULL
        && follower->active
        && selectedObject == follower;
}

static bool32 GoldenYellow_IsPokemonTowerPartnerInteractionMapP8(void)
{
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_POKEMON_TOWER_1F))
        return FALSE;

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

static bool32 GoldenYellow_IsPartnerMoodContextEligible(struct Pokemon *partner)
{
    struct ObjectEvent *follower;
    u8 reaction;

    if (partner == NULL || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER)
        return FALSE;

    // Mirror the accepted P7 resolver without consuming state: only its final
    // MOOD fallback is allowed to bridge into modern follower context.
    if (GoldenYellow_IsBillPartnerInteractionObjectP8())
        return FALSE;

    follower = GetFollowerObject();
    if (follower == NULL || !follower->active)
        return FALSE;

    if (GoldenYellow_IsFanClubPartnerParked(partner))
        return FALSE;

    if (GoldenYellow_IsPewterPartnerSleepActive(partner))
        return FALSE;

    if (GetMonData(partner, MON_DATA_STATUS) != STATUS1_NONE)
        return FALSE;

    if (GoldenYellow_IsPokemonTowerPartnerInteractionMapP8())
        return FALSE;

    if (GoldenYellow_TryGetPartnerPikachuOneShotReaction(partner, &reaction))
        return FALSE;

    return TRUE;
}

static bool8 GoldenYellow_WaitForPartnerMoodThenContext(void)
{
    struct ScriptContext *ctx;

    if (GoldenYellow_IsPartnerPikachuReactionActive())
        return FALSE;

    GoldenYellow_ClearPartnerPikachuReactionObject();

    ctx = sPartnerContextScript;
    sPartnerContextScript = NULL;
    if (ctx != NULL)
        GetFollowerContextAction(ctx);

    return TRUE;
}

void GoldenYellow_TryPartnerPikachuFieldInteraction(struct ScriptContext *ctx)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    bool32 moodContextEligible = GoldenYellow_IsPartnerMoodContextEligible(partner);

    sPartnerContextScript = NULL;
    GoldenYellow_TryPartnerPikachuFieldInteractionBase(ctx);

    if (gSpecialVar_Result == GY_PARTNER_FIELD_NOT_HANDLED)
        return;

    // Preserve P7's failure-safe ownership: every resolved Partner route is
    // exclusive unless a normal MOOD reaction actually started successfully.
    gSpecialVar_Result = GY_PARTNER_FIELD_EXCLUSIVE;
    if (!moodContextEligible || !GoldenYellow_IsPartnerPikachuReactionActive())
        return;

    gSpecialVar_Result = GY_PARTNER_FIELD_CONTEXT_ELIGIBLE;
    sPartnerContextScript = ctx;

    // Replace only the normal mood wait with the P8 continuation. The wrapper
    // waits for Yellow's portrait/reaction to finish first, then dispatches an
    // actually-selected modern context or silently jumps to FollowerEnd. This
    // keeps the existing script from reaching playfirstmoncry/getfolloweraction.
    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerMoodThenContext);
    ctx->waitAfterCallNative = TRUE;
}
