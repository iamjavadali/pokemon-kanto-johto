#include "global.h"
#include "battle.h"
#include "battle_main.h"
#include "event_data.h"
#include "golden_yellow_partner_state.h"
#include "item.h"
#include "main.h"
#include "overworld.h"
#include "pokemon.h"
#include "constants/battle.h"
#include "constants/game_stat.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/item_effects.h"
#include "constants/pokemon.h"
#include "constants/species.h"

// P5 claims three otherwise-unused saved event vars. Mood is encoded as
// mood + 1 so zero remains an uninitialized sentinel for old/dev saves.
#define VAR_GY_PARTNER_MOOD          VAR_UNUSED_0x40F7
#define VAR_GY_PARTNER_MOOD_STEP_LO  VAR_UNUSED_0x40F8
#define VAR_GY_PARTNER_MOOD_STEP_HI  VAR_UNUSED_0x40F9

#define PARTNER_INITIAL_FRIENDSHIP   90
#define PARTNER_MOOD_NEUTRAL         128
#define PARTNER_MOOD_AFTER_HEAL      131
#define PARTNER_MOOD_AFTER_LEVEL     138
#define PARTNER_MOOD_AFTER_TMHM      148
#define PARTNER_MOOD_AFTER_FAINT     108
#define PARTNER_MOOD_AFTER_PSN_FAINT 98
#define PARTNER_MOOD_AFTER_WIN       130

static const u8 sPartnerTalkReactionMatrix[7][5] =
{
    // Mood: 0-40, 41-127, 128, 129-210, 211-255.
    {GY_PARTNER_REACTION_VERY_ANGRY,         GY_PARTNER_REACTION_VERY_ANGRY,         GY_PARTNER_REACTION_IRRITATED,         GY_PARTNER_REACTION_ALOOF,           GY_PARTNER_REACTION_ALOOF},
    {GY_PARTNER_REACTION_STRONG_DISPLEASURE, GY_PARTNER_REACTION_STRONG_DISPLEASURE, GY_PARTNER_REACTION_DISPLEASED,       GY_PARTNER_REACTION_FROWNING,        GY_PARTNER_REACTION_FROWNING},
    {GY_PARTNER_REACTION_SUBDUED,            GY_PARTNER_REACTION_SUBDUED,            GY_PARTNER_REACTION_NEUTRAL,          GY_PARTNER_REACTION_HAPPY_GRIN,      GY_PARTNER_REACTION_HAPPY_GRIN},
    {GY_PARTNER_REACTION_SUBDUED,            GY_PARTNER_REACTION_SUBDUED,            GY_PARTNER_REACTION_PLAYFUL,          GY_PARTNER_REACTION_CONTENT,         GY_PARTNER_REACTION_CONTENT},
    {GY_PARTNER_REACTION_UNHAPPY,            GY_PARTNER_REACTION_UNHAPPY,            GY_PARTNER_REACTION_ENERGETIC_HAPPY,  GY_PARTNER_REACTION_HAPPY,           GY_PARTNER_REACTION_HAPPY},
    {GY_PARTNER_REACTION_UNHAPPY,            GY_PARTNER_REACTION_UNHAPPY,            GY_PARTNER_REACTION_STRONG_HAPPINESS, GY_PARTNER_REACTION_AFFECTION,       GY_PARTNER_REACTION_AFFECTION},
    {GY_PARTNER_REACTION_UNHAPPY,            GY_PARTNER_REACTION_UNHAPPY,            GY_PARTNER_REACTION_STRONG_AFFECTION, GY_PARTNER_REACTION_MAX_AFFECTION,   GY_PARTNER_REACTION_MAX_AFFECTION},
};

static bool32 IsPlayerPartnerPikachu(struct Pokemon *mon)
{
    if (mon == NULL)
        return FALSE;

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (mon == &gParties[B_TRAINER_PLAYER][i])
            return GetMonData(mon, MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER;
    }

    return FALSE;
}

static struct Pokemon *FindPlayerPartnerPikachu(void)
{
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];

        if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER)
            return mon;
    }

    return NULL;
}

static u32 GetCurrentStepCount(void)
{
    return GetGameStat(GAME_STAT_STEPS);
}

static u32 GetStoredMoodStepCount(void)
{
    return VarGet(VAR_GY_PARTNER_MOOD_STEP_LO)
         | ((u32)VarGet(VAR_GY_PARTNER_MOOD_STEP_HI) << 16);
}

static void SetStoredMoodStepCount(u32 steps)
{
    VarSet(VAR_GY_PARTNER_MOOD_STEP_LO, steps & 0xFFFF);
    VarSet(VAR_GY_PARTNER_MOOD_STEP_HI, steps >> 16);
}

static void SetStoredMood(u8 mood)
{
    VarSet(VAR_GY_PARTNER_MOOD, (u16)mood + 1);
}

static void EnsurePartnerStateInitialized(struct Pokemon *partner)
{
    if (!IsPlayerPartnerPikachu(partner) || VarGet(VAR_GY_PARTNER_MOOD) != 0)
        return;

    // Existing saves created before P5 used the expansion species default.
    // Preserve any friendship that has already moved away from that default,
    // but migrate untouched canonical Partners to Yellow's source value of 90.
    if (GetMonData(partner, MON_DATA_FRIENDSHIP) == gSpeciesInfo[SPECIES_PIKACHU_STARTER].friendship)
    {
        u32 friendship = PARTNER_INITIAL_FRIENDSHIP;
        SetMonData(partner, MON_DATA_FRIENDSHIP, &friendship);
    }

    SetStoredMood(PARTNER_MOOD_NEUTRAL);
    SetStoredMoodStepCount(GetCurrentStepCount());
}

u8 GoldenYellow_GetPartnerPikachuMood(struct Pokemon *partner)
{
    u32 currentSteps;
    u32 storedSteps;
    u32 elapsedSteps;
    u8 mood;

    if (!IsPlayerPartnerPikachu(partner))
        return PARTNER_MOOD_NEUTRAL;

    EnsurePartnerStateInitialized(partner);

    mood = VarGet(VAR_GY_PARTNER_MOOD) - 1;
    currentSteps = GetCurrentStepCount();
    storedSteps = GetStoredMoodStepCount();
    elapsedSteps = currentSteps - storedSteps;

    // Yellow moves wPikachuMood one point toward 128 on every player step.
    // Applying the elapsed steps lazily is observably identical when mood is
    // read/changed and avoids creating a second global overworld step system.
    if (mood < PARTNER_MOOD_NEUTRAL)
    {
        u32 distance = PARTNER_MOOD_NEUTRAL - mood;
        mood += elapsedSteps >= distance ? distance : elapsedSteps;
    }
    else if (mood > PARTNER_MOOD_NEUTRAL)
    {
        u32 distance = mood - PARTNER_MOOD_NEUTRAL;
        mood -= elapsedSteps >= distance ? distance : elapsedSteps;
    }

    SetStoredMood(mood);
    SetStoredMoodStepCount(currentSteps);
    return mood;
}

static void ApplyPartnerMoodTarget(struct Pokemon *partner, u8 target)
{
    u8 mood;

    if (!IsPlayerPartnerPikachu(partner))
        return;

    mood = GoldenYellow_GetPartnerPikachuMood(partner);

    if ((target > PARTNER_MOOD_NEUTRAL && mood < target)
     || (target < PARTNER_MOOD_NEUTRAL && mood > target))
        SetStoredMood(target);
}

static void EnsurePartnerMoodMinimum(struct Pokemon *partner, u8 minimum)
{
    u8 mood;

    if (!IsPlayerPartnerPikachu(partner))
        return;

    mood = GoldenYellow_GetPartnerPikachuMood(partner);
    if (mood < minimum)
        SetStoredMood(minimum);
}

u8 GoldenYellow_SelectPartnerTalkReactionForState(u8 friendship, u8 mood)
{
    u8 friendshipRow;
    u8 moodColumn;

    if (friendship <= 50)
        friendshipRow = 0;
    else if (friendship <= 100)
        friendshipRow = 1;
    else if (friendship <= 130)
        friendshipRow = 2;
    else if (friendship <= 160)
        friendshipRow = 3;
    else if (friendship <= 200)
        friendshipRow = 4;
    else if (friendship <= 250)
        friendshipRow = 5;
    else
        friendshipRow = 6;

    if (mood <= 40)
        moodColumn = 0;
    else if (mood <= 127)
        moodColumn = 1;
    else if (mood == PARTNER_MOOD_NEUTRAL)
        moodColumn = 2;
    else if (mood <= 210)
        moodColumn = 3;
    else
        moodColumn = 4;

    return sPartnerTalkReactionMatrix[friendshipRow][moodColumn];
}

u8 GoldenYellow_SelectPartnerTalkReaction(struct Pokemon *partner)
{
    u8 friendship;
    u8 mood;

    if (!IsPlayerPartnerPikachu(partner))
        return GY_PARTNER_REACTION_NEUTRAL;

    EnsurePartnerStateInitialized(partner);
    friendship = GetMonData(partner, MON_DATA_FRIENDSHIP);
    mood = GoldenYellow_GetPartnerPikachuMood(partner);

    return GoldenYellow_SelectPartnerTalkReactionForState(friendship, mood);
}

void GoldenYellow_DebugSetPartnerPikachuState(struct Pokemon *partner, u8 friendship, u8 mood)
{
    u32 friendshipValue = friendship;

    if (!IsPlayerPartnerPikachu(partner))
        return;

    SetMonData(partner, MON_DATA_FRIENDSHIP, &friendshipValue);
    SetStoredMood(mood);
    SetStoredMoodStepCount(GetCurrentStepCount());
}

void AdjustFriendship(struct Pokemon *mon, u8 event)
{
    bool32 isPartner = IsPlayerPartnerPikachu(mon);
    bool32 skipPartnerState = isPartner && ShouldSkipFriendshipChange();

    if (isPartner && !skipPartnerState)
        EnsurePartnerStateInitialized(mon);

    GoldenYellow_AdjustFriendshipBase(mon, event);

    if (!isPartner || skipPartnerState)
        return;

    switch (event)
    {
    case FRIENDSHIP_EVENT_GROW_LEVEL:
        ApplyPartnerMoodTarget(mon, PARTNER_MOOD_AFTER_LEVEL);
        break;
    case FRIENDSHIP_EVENT_LEARN_TMHM:
        ApplyPartnerMoodTarget(mon, PARTNER_MOOD_AFTER_TMHM);
        break;
    case FRIENDSHIP_EVENT_FAINT_SMALL:
        ApplyPartnerMoodTarget(mon, PARTNER_MOOD_AFTER_FAINT);
        break;
    case FRIENDSHIP_EVENT_FAINT_FIELD_PSN:
        ApplyPartnerMoodTarget(mon, PARTNER_MOOD_AFTER_PSN_FAINT);
        break;
    case FRIENDSHIP_EVENT_FAINT_LARGE:
        ApplyPartnerMoodTarget(mon, PARTNER_MOOD_AFTER_FAINT);
        break;
    }
}

static void ApplyPartnerHealingFriendship(struct Pokemon *partner, u8 friendshipBefore)
{
    enum Item heldItem = GetMonData(partner, MON_DATA_HELD_ITEM);
    enum HoldEffect holdEffect = GetItemHoldEffect(heldItem);
    s32 modifier;
    s32 friendship;

    if (friendshipBefore < 100)
        modifier = 5;
    else if (friendshipBefore < 200)
        modifier = 3;
    else
        modifier = 2;

    friendship = GetMonData(partner, MON_DATA_FRIENDSHIP);
    friendship += CalculateFriendshipBonuses(partner, modifier, holdEffect);

    if (friendship > MAX_FRIENDSHIP)
        friendship = MAX_FRIENDSHIP;

    SetMonData(partner, MON_DATA_FRIENDSHIP, &friendship);
}

bool8 ExecuteTableBasedItemEffect(struct Pokemon *mon, enum Item item, u8 partyIndex, u8 moveIndex)
{
    const u8 *itemEffect = GetItemEffect(item);
    bool32 isPartner = IsPlayerPartnerPikachu(mon);
    bool32 skipPartnerState = isPartner && ShouldSkipFriendshipChange();
    u32 hpBefore = 0;
    u8 friendshipBefore = 0;
    bool8 hadNoEffect;

    if (isPartner && !skipPartnerState)
    {
        EnsurePartnerStateInitialized(mon);
        hpBefore = GetMonData(mon, MON_DATA_HP);
        friendshipBefore = GetMonData(mon, MON_DATA_FRIENDSHIP);
    }

    hadNoEffect = GoldenYellow_ExecuteTableBasedItemEffectBase(mon, item, partyIndex, moveIndex);

    if (isPartner
     && !skipPartnerState
     && !hadNoEffect
     && itemEffect != NULL
     && (itemEffect[4] & ITEM4_HEAL_HP)
     && !(itemEffect[5] & ITEM5_FRIENDSHIP_ALL)
     && GetMonData(mon, MON_DATA_HP) > hpBefore)
    {
        // Add Yellow's medicine relationship event only when the modern item
        // does not already carry its own explicit friendship effect.
        ApplyPartnerHealingFriendship(mon, friendshipBefore);
        ApplyPartnerMoodTarget(mon, PARTNER_MOOD_AFTER_HEAL);
    }

    return hadNoEffect;
}

void ZeroEnemyPartyMons(void)
{
    // Standard battle teardown reaches this after gBattleOutcome is final. Keep
    // Yellow's post-win mood boost out of link/frontier contexts, where the
    // expansion already suppresses persistent friendship changes.
    if (gMain.callback2 == BattleMainCB2
     && gBattleOutcome == B_OUTCOME_WON
     && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER)))
    {
        struct Pokemon *partner = FindPlayerPartnerPikachu();

        if (partner != NULL && GetMonData(partner, MON_DATA_HP) != 0)
        {
            EnsurePartnerStateInitialized(partner);
            EnsurePartnerMoodMinimum(partner, PARTNER_MOOD_AFTER_WIN);
        }
    }

    GoldenYellow_ZeroEnemyPartyMonsBase();
}
