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
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"

// fishing.c deliberately owns the native implementation and does not include
// fishing.h, allowing the public header to route callers through the P6 wrapper.
void StartFishing(u8 rod);

// P5 claims three otherwise-unused saved event vars. Mood is encoded as
// mood + 1 so zero remains an uninitialized sentinel for old/dev saves.
#define VAR_GY_PARTNER_MOOD              VAR_UNUSED_0x40F7
#define VAR_GY_PARTNER_MOOD_STEP_LO      VAR_UNUSED_0x40F8
#define VAR_GY_PARTNER_MOOD_STEP_HI      VAR_UNUSED_0x40F9
// P6 mirrors Yellow's single wPikachuEmotionModifier byte. Zero is no override.
#define VAR_GY_PARTNER_REACTION_MODIFIER VAR_UNUSED_0x40FA

#define PARTNER_INITIAL_FRIENDSHIP       90
#define PARTNER_MOOD_NEUTRAL             128
#define PARTNER_MOOD_AFTER_HEAL          131
#define PARTNER_MOOD_AFTER_LEVEL         138
#define PARTNER_MOOD_AFTER_TMHM          148
#define PARTNER_MOOD_AFTER_ELECTRIC_MOVE 133
#define PARTNER_MOOD_AFTER_FAINT         108
#define PARTNER_MOOD_AFTER_PSN_FAINT     98
#define PARTNER_MOOD_AFTER_WIN           130

enum GoldenYellowPartnerReactionModifier
{
    GY_PARTNER_MODIFIER_NONE = 0,
    GY_PARTNER_MODIFIER_CAPTURE = 1,
    GY_PARTNER_MODIFIER_FISHING = 2,
    GY_PARTNER_MODIFIER_RESERVED_3 = 3,
    GY_PARTNER_MODIFIER_THUNDER_STONE = 4,
    GY_PARTNER_MODIFIER_ELECTRIC_POWER = 5,
};

// Replacement move flows first report MON_HAS_MAX_MOVES, then commit the chosen
// slot later. Keep that in-flight state volatile; only the resulting one-shot
// modifier belongs in the save block.
static EWRAM_DATA struct BoxPokemon *sPendingPartnerElectricBox = NULL;
static EWRAM_DATA enum Move sPendingPartnerElectricMove = MOVE_NONE;

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

static struct Pokemon *FindPlayerPartnerPikachuByBox(struct BoxPokemon *boxMon)
{
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];

        if (&mon->box == boxMon && GetMonData(mon, MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER)
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

static void SetPartnerReactionModifier(struct Pokemon *partner, u8 modifier)
{
    if (!IsPlayerPartnerPikachu(partner))
        return;

    EnsurePartnerStateInitialized(partner);
    VarSet(VAR_GY_PARTNER_REACTION_MODIFIER, modifier);
}

static bool32 IsPartnerElectricPowerMove(enum Move move)
{
    return move == MOVE_THUNDERBOLT || move == MOVE_THUNDER;
}

static void ClearPendingPartnerElectricMove(void)
{
    sPendingPartnerElectricBox = NULL;
    sPendingPartnerElectricMove = MOVE_NONE;
}

static void ApplyPartnerElectricPowerEvent(struct Pokemon *partner)
{
    if (!IsPlayerPartnerPikachu(partner))
        return;

    // Source behavior is an exact assignment to 0x85, not a directional P5
    // target. Advance lazy step decay first, then establish the new event mood.
    GoldenYellow_GetPartnerPikachuMood(partner);
    SetStoredMood(PARTNER_MOOD_AFTER_ELECTRIC_MOVE);
    SetStoredMoodStepCount(GetCurrentStepCount());
    SetPartnerReactionModifier(partner, GY_PARTNER_MODIFIER_ELECTRIC_POWER);
}

static void HandlePartnerMoveLearningResult(struct Pokemon *partner, enum Move move, u16 result)
{
    if (!IsPlayerPartnerPikachu(partner))
        return;

    ClearPendingPartnerElectricMove();
    if (!IsPartnerElectricPowerMove(move))
        return;

    if (result == move)
    {
        ApplyPartnerElectricPowerEvent(partner);
    }
    else if (result == MON_HAS_MAX_MOVES)
    {
        sPendingPartnerElectricBox = &partner->box;
        sPendingPartnerElectricMove = move;
    }
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

bool8 GoldenYellow_TryGetPartnerPikachuOneShotReaction(struct Pokemon *partner, u8 *reaction)
{
    if (!IsPlayerPartnerPikachu(partner) || reaction == NULL)
        return FALSE;

    EnsurePartnerStateInitialized(partner);
    switch (VarGet(VAR_GY_PARTNER_REACTION_MODIFIER))
    {
    case GY_PARTNER_MODIFIER_CAPTURE:
        *reaction = GY_PARTNER_REACTION_CAPTURE_SUCCESS;
        return TRUE;
    case GY_PARTNER_MODIFIER_FISHING:
        *reaction = GY_PARTNER_REACTION_FISHING;
        return TRUE;
    case GY_PARTNER_MODIFIER_RESERVED_3:
        // Yellow maps modifier 3 to Emotion23. Its gameplay writer remains
        // deliberately unimplemented until a canonical source trigger is found.
        *reaction = GY_PARTNER_REACTION_BILL_CONFUSED;
        return TRUE;
    case GY_PARTNER_MODIFIER_THUNDER_STONE:
        *reaction = GY_PARTNER_REACTION_THUNDER_STONE_REFUSAL;
        return TRUE;
    case GY_PARTNER_MODIFIER_ELECTRIC_POWER:
        *reaction = GY_PARTNER_REACTION_ELECTRIC_POWER;
        return TRUE;
    default:
        return FALSE;
    }
}

void GoldenYellow_ConsumePartnerPikachuOneShotReaction(struct Pokemon *partner)
{
    if (IsPlayerPartnerPikachu(partner))
        VarSet(VAR_GY_PARTNER_REACTION_MODIFIER, GY_PARTNER_MODIFIER_NONE);
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
    {
        enum Move taughtMove = ItemIdToBattleMoveId(gSpecialVar_ItemId);

        // TM learning reports friendship after the move has already been
        // committed. Reassert Yellow's move-specific 0x85 mood after the
        // generic P5 TM target so Thunderbolt/Thunder finish at source value.
        if (IsPartnerElectricPowerMove(taughtMove))
            ApplyPartnerElectricPowerEvent(mon);
        else
            ApplyPartnerMoodTarget(mon, PARTNER_MOOD_AFTER_TMHM);
        break;
    }
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

    // Yellow's Partner refuses Thunder Stone evolution. Claim the actual item
    // attempt here before the generic evolution-item effect can mutate or
    // consume anything; the normal party callback will show its no-effect path.
    if (isPartner && item == ITEM_THUNDER_STONE)
    {
        SetPartnerReactionModifier(mon, GY_PARTNER_MODIFIER_THUNDER_STONE);
        return TRUE;
    }

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

u16 GiveMoveToMon(struct Pokemon *mon, enum Move move)
{
    u16 result = GoldenYellow_GiveMoveToMonBase(mon, move);

    HandlePartnerMoveLearningResult(mon, move, result);
    return result;
}

u16 GiveMoveToBoxMon(struct BoxPokemon *boxMon, enum Move move)
{
    struct Pokemon *partner = FindPlayerPartnerPikachuByBox(boxMon);
    u16 result = GoldenYellow_GiveMoveToBoxMonBase(boxMon, move);

    HandlePartnerMoveLearningResult(partner, move, result);
    return result;
}

enum Move MonTryLearningNewMoveAtLevel(struct Pokemon *mon, bool32 firstMove, u32 level)
{
    enum Move result = GoldenYellow_MonTryLearningNewMoveAtLevelBase(mon, firstMove, level);

    HandlePartnerMoveLearningResult(mon, gMoveToLearn, result);
    return result;
}

enum Move MonTryLearningNewMove(struct Pokemon *mon, bool8 firstMove)
{
    enum Move result = GoldenYellow_MonTryLearningNewMoveBase(mon, firstMove);

    HandlePartnerMoveLearningResult(mon, gMoveToLearn, result);
    return result;
}

void SetMonMoveSlot(struct Pokemon *mon, enum Move move, u8 slot)
{
    bool32 isPartner = IsPlayerPartnerPikachu(mon);
    bool32 confirmsPending = isPartner
                          && sPendingPartnerElectricBox == &mon->box
                          && sPendingPartnerElectricMove == move;

    GoldenYellow_SetMonMoveSlotBase(mon, move, slot);

    if (confirmsPending && GetMonData(mon, MON_DATA_MOVE1 + slot) == move)
        ApplyPartnerElectricPowerEvent(mon);

    if (isPartner && sPendingPartnerElectricBox == &mon->box)
        ClearPendingPartnerElectricMove();
}

void SetBoxMonData(struct BoxPokemon *boxMon, s32 field, const void *dataArg)
{
    struct Pokemon *partner = FindPlayerPartnerPikachuByBox(boxMon);
    bool32 isMoveField = field >= MON_DATA_MOVE1 && field <= MON_DATA_MOVE4;
    enum Move move = MOVE_NONE;
    bool32 confirmsPending = FALSE;

    if (partner != NULL && isMoveField && dataArg != NULL)
    {
        move = *(const u16 *)dataArg;
        confirmsPending = sPendingPartnerElectricBox == boxMon
                       && sPendingPartnerElectricMove == move;
    }

    GoldenYellow_SetBoxMonDataBase(boxMon, field, dataArg);

    if (confirmsPending && GetBoxMonData(boxMon, field) == move)
        ApplyPartnerElectricPowerEvent(partner);

    if (partner != NULL && isMoveField && sPendingPartnerElectricBox == boxMon)
        ClearPendingPartnerElectricMove();
}

void GoldenYellow_StartFishing(u8 rod)
{
    struct Pokemon *partner;

    StartFishing(rod);
    partner = FindPlayerPartnerPikachu();
    if (partner != NULL)
        SetPartnerReactionModifier(partner, GY_PARTNER_MODIFIER_FISHING);
}

void ZeroEnemyPartyMons(void)
{
    // Standard battle teardown reaches this after gBattleOutcome is final. Keep
    // Yellow state changes out of link/frontier contexts, matching P5's existing
    // persistence rule.
    if (gMain.callback2 == BattleMainCB2
     && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER)))
    {
        struct Pokemon *partner = FindPlayerPartnerPikachu();

        if (partner != NULL)
        {
            if (gBattleOutcome == B_OUTCOME_CAUGHT)
            {
                SetPartnerReactionModifier(partner, GY_PARTNER_MODIFIER_CAPTURE);
            }
            else if (gBattleOutcome == B_OUTCOME_WON && GetMonData(partner, MON_DATA_HP) != 0)
            {
                EnsurePartnerStateInitialized(partner);
                EnsurePartnerMoodMinimum(partner, PARTNER_MOOD_AFTER_WIN);
            }
        }
    }

    GoldenYellow_ZeroEnemyPartyMonsBase();
}
