#include "global.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "field_emote_icons.h"
#include "field_player_avatar.h"
#include "follower_helper.h"
#include "golden_yellow_partner_reaction.h"
#include "main.h"
#include "pokemon.h"
#include "script.h"
#include "sound.h"
#include "task.h"
#include "constants/event_object_movement.h"
#include "constants/field_effects.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/species.h"

#define PARTNER_REACTION_NO_FIELD_EFFECT (-1)
#define PARTNER_REACTION_TURN_AWAY_MOVEMENT 0xFE

enum GoldenYellowPartnerReactionTaskMode
{
    GY_PARTNER_REACTION_MODE_ONESHOT,
    GY_PARTNER_REACTION_MODE_DEBUG_BROWSER,
};

enum GoldenYellowPartnerReactionTaskState
{
    GY_PARTNER_REACTION_STATE_RUN_COMMAND,
    GY_PARTNER_REACTION_STATE_WAIT_CRY,
    GY_PARTNER_REACTION_STATE_WAIT_BUBBLE,
    GY_PARTNER_REACTION_STATE_WAIT_MOVEMENT,
    GY_PARTNER_REACTION_STATE_WAIT_PORTRAIT,
    GY_PARTNER_REACTION_STATE_WAIT_DELAY,
    GY_PARTNER_REACTION_STATE_BROWSER_IDLE,
};

struct GoldenYellowPartnerReactionCommand
{
    u8 type;
    u8 arg;
    u16 value;
};

#include "golden_yellow_partner_reaction_data.inc"

#define rReactionId     data[0]
#define rCommandIndex   data[1]
#define rState          data[2]
#define rMode           data[3]
#define rWaitTimer      data[4]
#define rMovementId     data[5]
#define rMovementStep   data[6]
#define rInputCooldown  data[7]
#define rBubbleEffectId data[8]

static void Task_PartnerReaction(u8 taskId);
static bool32 StartPartnerReactionTask(u8 reactionId, enum GoldenYellowPartnerReactionTaskMode mode);
static bool32 HasFreePartnerReactionTaskSlot(void);
static bool32 IsCanonicalPartnerFollowerReady(struct ObjectEvent **follower);
static void BeginPartnerReaction(u8 taskId, u8 reactionId);
static void FinishPartnerReaction(u8 taskId);
static void ClosePartnerReactionBrowser(u8 taskId);
static void ExecutePartnerReactionCommand(u8 taskId);
static void ExecutePartnerReactionCallback(u8 callbackId);
static bool32 StartPartnerReactionBubble(u8 taskId, u8 bubbleId);
static bool32 IsPartnerReactionBubbleActive(u8 taskId);
static s16 GetFollowerEmotionForBubble(u8 bubbleId);
static void StartPartnerReactionMovement(u8 taskId, u8 movementId);
static bool32 UpdatePartnerReactionMovement(u8 taskId);
static enum Direction GetFollowerDirectionTowardPlayer(const struct ObjectEvent *follower);
static enum Direction GetFollowerDirectionAwayFromPlayer(const struct ObjectEvent *follower);

static bool32 HasFreePartnerReactionTaskSlot(void)
{
    u8 taskId;

    for (taskId = 0; taskId < NUM_TASKS; taskId++)
    {
        if (!gTasks[taskId].isActive)
            return TRUE;
    }

    return FALSE;
}

static bool32 IsCanonicalPartnerFollowerReady(struct ObjectEvent **follower)
{
    struct Pokemon *partner = GetPartnerAwareFollowingMon();
    struct ObjectEvent *followerObject = GetFollowerObject();

    if (partner == NULL
     || GetMonData(partner, MON_DATA_SPECIES) != SPECIES_PIKACHU_STARTER
     || followerObject == NULL
     || !followerObject->active)
        return FALSE;

    if (follower != NULL)
        *follower = followerObject;

    return TRUE;
}

static bool32 StartPartnerReactionTask(u8 reactionId, enum GoldenYellowPartnerReactionTaskMode mode)
{
    struct ObjectEvent *follower;
    u8 taskId;

    if (reactionId >= GY_PARTNER_REACTION_COUNT
     || FindTaskIdByFunc(Task_PartnerReaction) != TASK_NONE
     || GoldenYellow_IsPartnerPikachuPortraitActive()
     || !HasFreePartnerReactionTaskSlot()
     || !IsCanonicalPartnerFollowerReady(&follower))
        return FALSE;

    // Debug/script locks freeze the world. The Partner is the only object the
    // reaction director needs to animate, so keep it under the existing
    // follower engine while the rest of the map remains frozen.
    UnfreezeObjectEvent(follower);

    taskId = CreateTask(Task_PartnerReaction, 0x51);
    gTasks[taskId].rMode = mode;
    gTasks[taskId].rBubbleEffectId = PARTNER_REACTION_NO_FIELD_EFFECT;
    BeginPartnerReaction(taskId, reactionId);
    return TRUE;
}

bool32 GoldenYellow_StartPartnerPikachuReaction(u8 reactionId)
{
    return StartPartnerReactionTask(reactionId, GY_PARTNER_REACTION_MODE_ONESHOT);
}

bool32 GoldenYellow_IsPartnerPikachuReactionActive(void)
{
    return FindTaskIdByFunc(Task_PartnerReaction) != TASK_NONE;
}

bool32 GoldenYellow_DebugStartPikachuReactionBrowser(void)
{
    // Controls while the reaction director is idle:
    //   LEFT/RIGHT  previous/next Yellow Emotion (0-32)
    //   A           replay current Emotion
    //   B           close browser
    //
    // A/B belong to a child portrait while that portrait is visible. A short
    // input cooldown prevents the same key edge from also replaying/exiting the
    // parent browser after the child closes.
    return StartPartnerReactionTask(GY_PARTNER_REACTION_EMPTY, GY_PARTNER_REACTION_MODE_DEBUG_BROWSER);
}

static void BeginPartnerReaction(u8 taskId, u8 reactionId)
{
    struct Task *task = &gTasks[taskId];

    task->rReactionId = reactionId;
    task->rCommandIndex = 0;
    task->rState = GY_PARTNER_REACTION_STATE_RUN_COMMAND;
    task->rWaitTimer = 0;
    task->rMovementId = 0;
    task->rMovementStep = 0;
    task->rBubbleEffectId = PARTNER_REACTION_NO_FIELD_EFFECT;
}

static void FinishPartnerReaction(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->rMode == GY_PARTNER_REACTION_MODE_DEBUG_BROWSER)
    {
        task->rState = GY_PARTNER_REACTION_STATE_BROWSER_IDLE;
        task->rInputCooldown = 2;
    }
    else
    {
        DestroyTask(taskId);
    }
}

static void ClosePartnerReactionBrowser(u8 taskId)
{
    struct ObjectEvent *follower = GetFollowerObject();

    if (follower != NULL && follower->active)
        ObjectEventClearHeldMovementIfActive(follower);

    DestroyTask(taskId);
    UnlockPlayerFieldControls();
    UnfreezeObjectEvents();
    ScriptContext_Enable();
}

static void Task_PartnerReaction(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    bool32 acceptBrowserInput = task->rInputCooldown == 0;

    if (task->rInputCooldown > 0)
        task->rInputCooldown--;

    switch (task->rState)
    {
    case GY_PARTNER_REACTION_STATE_BROWSER_IDLE:
        if (!acceptBrowserInput)
            return;

        if (gMain.newKeys & B_BUTTON)
        {
            ClosePartnerReactionBrowser(taskId);
            return;
        }

        if (gMain.newKeys & DPAD_RIGHT)
        {
            BeginPartnerReaction(taskId, (task->rReactionId + 1) % GY_PARTNER_REACTION_COUNT);
            return;
        }

        if (gMain.newKeys & DPAD_LEFT)
        {
            u8 reactionId = task->rReactionId == 0
                          ? GY_PARTNER_REACTION_COUNT - 1
                          : task->rReactionId - 1;

            BeginPartnerReaction(taskId, reactionId);
            return;
        }

        if (gMain.newKeys & A_BUTTON)
        {
            BeginPartnerReaction(taskId, task->rReactionId);
            return;
        }
        break;

    case GY_PARTNER_REACTION_STATE_WAIT_CRY:
        if (IsCryFinished())
        {
            task->rCommandIndex++;
            task->rState = GY_PARTNER_REACTION_STATE_RUN_COMMAND;
        }
        break;

    case GY_PARTNER_REACTION_STATE_WAIT_BUBBLE:
        if (!IsPartnerReactionBubbleActive(taskId))
        {
            task->rBubbleEffectId = PARTNER_REACTION_NO_FIELD_EFFECT;
            task->rCommandIndex++;
            task->rState = GY_PARTNER_REACTION_STATE_RUN_COMMAND;
        }
        break;

    case GY_PARTNER_REACTION_STATE_WAIT_MOVEMENT:
        if (UpdatePartnerReactionMovement(taskId))
        {
            task->rCommandIndex++;
            task->rState = GY_PARTNER_REACTION_STATE_RUN_COMMAND;
        }
        break;

    case GY_PARTNER_REACTION_STATE_WAIT_PORTRAIT:
        if (!GoldenYellow_IsPartnerPikachuPortraitActive())
        {
            task->rCommandIndex++;
            task->rState = GY_PARTNER_REACTION_STATE_RUN_COMMAND;
            task->rInputCooldown = 2;
        }
        break;

    case GY_PARTNER_REACTION_STATE_WAIT_DELAY:
        if (task->rWaitTimer > 0)
            task->rWaitTimer--;

        if (task->rWaitTimer == 0)
        {
            task->rCommandIndex++;
            task->rState = GY_PARTNER_REACTION_STATE_RUN_COMMAND;
        }
        break;

    case GY_PARTNER_REACTION_STATE_RUN_COMMAND:
        ExecutePartnerReactionCommand(taskId);
        break;
    }
}

static void ExecutePartnerReactionCommand(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    const struct GoldenYellowPartnerReactionCommand *command =
        &sPartnerReactionPrograms[task->rReactionId][task->rCommandIndex];

    switch (command->type)
    {
    case GY_PARTNER_REACTION_CMD_END:
        FinishPartnerReaction(taskId);
        break;

    case GY_PARTNER_REACTION_CMD_CRY:
        // P3 preserves each Yellow semantic cry ID in the command data. Until
        // the dedicated audio pass, every requested clip uses the modern
        // Partner Pikachu cry as the approved fallback.
        PlayCry_Normal(SPECIES_PIKACHU_STARTER, 0);
        task->rState = GY_PARTNER_REACTION_STATE_WAIT_CRY;
        break;

    case GY_PARTNER_REACTION_CMD_BUBBLE:
        if (StartPartnerReactionBubble(taskId, command->arg))
            task->rState = GY_PARTNER_REACTION_STATE_WAIT_BUBBLE;
        else
            task->rCommandIndex++;
        break;

    case GY_PARTNER_REACTION_CMD_MOVEMENT:
        StartPartnerReactionMovement(taskId, command->arg);
        task->rState = GY_PARTNER_REACTION_STATE_WAIT_MOVEMENT;
        break;

    case GY_PARTNER_REACTION_CMD_PORTRAIT:
        if (GoldenYellow_StartPartnerPikachuPortraitForReaction(command->arg))
            task->rState = GY_PARTNER_REACTION_STATE_WAIT_PORTRAIT;
        else
            task->rCommandIndex++;
        break;

    case GY_PARTNER_REACTION_CMD_DELAY:
        task->rWaitTimer = command->value;
        if (task->rWaitTimer == 0)
            task->rCommandIndex++;
        else
            task->rState = GY_PARTNER_REACTION_STATE_WAIT_DELAY;
        break;

    case GY_PARTNER_REACTION_CMD_TURN_AWAY:
        StartPartnerReactionMovement(taskId, PARTNER_REACTION_TURN_AWAY_MOVEMENT);
        task->rState = GY_PARTNER_REACTION_STATE_WAIT_MOVEMENT;
        break;

    case GY_PARTNER_REACTION_CMD_CALLBACK:
        ExecutePartnerReactionCallback(command->arg);
        task->rCommandIndex++;
        break;

    default:
        // Corrupt data must never strand field controls.
        FinishPartnerReaction(taskId);
        break;
    }
}

static void ExecutePartnerReactionCallback(u8 callbackId)
{
    // P3 records source subcommand semantics without advancing story state.
    // Authored-scene integration will bind these IDs to the existing
    // accepted Pewter/Bill choreography and the future Fan Club sequence.
    (void)callbackId;
}

static s16 GetFollowerEmotionForBubble(u8 bubbleId)
{
    switch (bubbleId)
    {
    case GY_PARTNER_BUBBLE_FOLLOWER_HAPPY:
        return FOLLOWER_EMOTION_HAPPY;
    case GY_PARTNER_BUBBLE_FOLLOWER_NEUTRAL:
        return FOLLOWER_EMOTION_NEUTRAL;
    case GY_PARTNER_BUBBLE_FOLLOWER_SAD:
        return FOLLOWER_EMOTION_SAD;
    case GY_PARTNER_BUBBLE_FOLLOWER_UPSET:
        return FOLLOWER_EMOTION_UPSET;
    case GY_PARTNER_BUBBLE_FOLLOWER_ANGRY:
        return FOLLOWER_EMOTION_ANGRY;
    case GY_PARTNER_BUBBLE_FOLLOWER_PENSIVE:
        return FOLLOWER_EMOTION_PENSIVE;
    case GY_PARTNER_BUBBLE_FOLLOWER_LOVE:
        return FOLLOWER_EMOTION_LOVE;
    case GY_PARTNER_BUBBLE_FOLLOWER_SURPRISE:
        return FOLLOWER_EMOTION_SURPRISE;
    case GY_PARTNER_BUBBLE_FOLLOWER_CURIOUS:
        return FOLLOWER_EMOTION_CURIOUS;
    case GY_PARTNER_BUBBLE_FOLLOWER_MUSIC:
        return FOLLOWER_EMOTION_MUSIC;
    case GY_PARTNER_BUBBLE_FOLLOWER_POISONED:
        return FOLLOWER_EMOTION_POISONED;
    default:
        return -1;
    }
}

static bool32 StartPartnerReactionBubble(u8 taskId, u8 bubbleId)
{
    struct Task *task = &gTasks[taskId];
    struct ObjectEvent *follower = GetFollowerObject();
    s16 followerEmotion = GetFollowerEmotionForBubble(bubbleId);
    enum FieldEmoteIcon sharedIcon;

    if (follower == NULL || !follower->active)
        return FALSE;

    task->rBubbleEffectId = PARTNER_REACTION_NO_FIELD_EFFECT;

    // First choice: reuse the expansion's existing animated HGSS follower
    // emotion renderer exactly as ordinary follower interactions do.
    if (followerEmotion >= 0)
    {
        if (FieldEffectActiveListContains(FLDEFF_EMOTE))
            return FALSE;

        gFieldEffectArguments[0] = follower->localId;
        gFieldEffectArguments[1] = gSaveBlock1Ptr->location.mapNum;
        gFieldEffectArguments[2] = gSaveBlock1Ptr->location.mapGroup;
        gFieldEffectArguments[7] = followerEmotion;
        FieldEffectStart(FLDEFF_EMOTE);
        task->rBubbleEffectId = FLDEFF_EMOTE;
        return TRUE;
    }

    // Yellow-only concepts stay in the shared field-emote subsystem so they
    // remain reusable by other followers/NPCs rather than becoming
    // Partner-Pikachu-private presentation code.
    switch (bubbleId)
    {
    case GY_PARTNER_BUBBLE_SKULL:
        sharedIcon = FIELD_EMOTE_SKULL;
        break;
    case GY_PARTNER_BUBBLE_ZZZ:
        sharedIcon = FIELD_EMOTE_SLEEP;
        break;
    case GY_PARTNER_BUBBLE_BOLT:
        sharedIcon = FIELD_EMOTE_BOLT;
        break;
    case GY_PARTNER_BUBBLE_FISH:
        sharedIcon = FIELD_EMOTE_FISH;
        break;
    case GY_PARTNER_BUBBLE_ANIMATED_EXCLAMATION:
        sharedIcon = FIELD_EMOTE_ANIMATED_EXCLAMATION;
        break;
    default:
        return FALSE;
    }

    if (!FieldEmote_StartOnObjectEvent(follower, sharedIcon))
        return FALSE;

    task->rBubbleEffectId = FLDEFF_SHARED_EMOTE_ICON;
    return TRUE;
}

static bool32 IsPartnerReactionBubbleActive(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->rBubbleEffectId == PARTNER_REACTION_NO_FIELD_EFFECT)
        return FALSE;

    return FieldEffectActiveListContains(task->rBubbleEffectId);
}

static enum Direction GetFollowerDirectionTowardPlayer(const struct ObjectEvent *follower)
{
    const struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    s16 deltaX = player->currentCoords.x - follower->currentCoords.x;
    s16 deltaY = player->currentCoords.y - follower->currentCoords.y;

    if (deltaX > 0)
        return DIR_EAST;
    if (deltaX < 0)
        return DIR_WEST;
    if (deltaY > 0)
        return DIR_SOUTH;
    if (deltaY < 0)
        return DIR_NORTH;

    return follower->facingDirection;
}

static enum Direction GetFollowerDirectionAwayFromPlayer(const struct ObjectEvent *follower)
{
    return GetOppositeDirection(GetFollowerDirectionTowardPlayer(follower));
}

static void StartPartnerReactionMovement(u8 taskId, u8 movementId)
{
    struct Task *task = &gTasks[taskId];

    task->rMovementId = movementId;
    task->rMovementStep = 0;
    task->rWaitTimer = 0;
}

static bool32 UpdatePartnerReactionMovement(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    struct ObjectEvent *follower = GetFollowerObject();
    enum Direction towardPlayer;
    enum Direction awayFromPlayer;

    if (follower == NULL || !follower->active)
        return TRUE;

    if (ObjectEventIsHeldMovementActive(follower))
    {
        if (!ObjectEventClearHeldMovementIfFinished(follower))
            return FALSE;
    }

    if (task->rWaitTimer > 0)
    {
        task->rWaitTimer--;
        return FALSE;
    }

    towardPlayer = GetFollowerDirectionTowardPlayer(follower);
    awayFromPlayer = GetFollowerDirectionAwayFromPlayer(follower);

    switch (task->rMovementId)
    {
    case GY_PARTNER_MOVEMENT_ENERGETIC_HOP: // Yellow fd224
        if (task->rMovementStep < 2)
        {
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(follower->facingDirection));
            task->rMovementStep++;
            return FALSE;
        }
        return TRUE;

    case GY_PARTNER_MOVEMENT_PLAYFUL_HOP: // Yellow fd230
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(follower->facingDirection));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            task->rWaitTimer = 8;
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(follower->facingDirection));
            task->rMovementStep = 3;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_ALOOF_TURN: // Yellow fd21e
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(follower->facingDirection, TRUE)));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            task->rWaitTimer = 30;
            task->rMovementStep = 2;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_AGITATED_TURN: // Yellow fd218
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(follower->facingDirection, TRUE)));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            task->rWaitTimer = 4;
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(follower->facingDirection, TRUE)));
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            task->rWaitTimer = 30;
            task->rMovementStep = 4;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_HAPPY_BOUNCE:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
        case 3:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(towardPlayer));
            task->rMovementStep++;
            return FALSE;
        case 2:
            task->rWaitTimer = 4;
            task->rMovementStep = 3;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_AFFECTION_BOUNCE:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(towardPlayer));
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            task->rWaitTimer = 8;
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(towardPlayer));
            task->rMovementStep = 4;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_CELEBRATE:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(DIR_NORTH));
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(DIR_SOUTH));
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(DIR_WEST));
            task->rMovementStep = 4;
            return FALSE;
        case 4:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(DIR_EAST));
            task->rMovementStep = 5;
            return FALSE;
        case 5:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 6;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_CONFUSED_LOOK:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, TRUE)));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            task->rWaitTimer = 8;
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, FALSE)));
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            task->rWaitTimer = 8;
            task->rMovementStep = 4;
            return FALSE;
        case 4:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 5;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_REFUSAL_SHAKE:
        switch (task->rMovementStep)
        {
        case 0:
        case 4:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, TRUE)));
            task->rMovementStep++;
            return FALSE;
        case 2:
        case 6:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, FALSE)));
            task->rMovementStep++;
            return FALSE;
        case 1:
        case 3:
        case 5:
        case 7:
            task->rWaitTimer = 4;
            task->rMovementStep++;
            return FALSE;
        case 8:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 9;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_FEAR_RECOIL:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(awayFromPlayer));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(awayFromPlayer));
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            task->rWaitTimer = 8;
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(awayFromPlayer));
            task->rMovementStep = 4;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_ELECTRIC_JOLT:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(
                follower,
                GetJumpInPlaceMovementAction(GetNinetyDegreeDirection(towardPlayer, TRUE)));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            ObjectEventSetHeldMovement(
                follower,
                GetJumpInPlaceMovementAction(GetNinetyDegreeDirection(towardPlayer, FALSE)));
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            ObjectEventSetHeldMovement(
                follower,
                GetJumpInPlaceMovementAction(GetNinetyDegreeDirection(towardPlayer, TRUE)));
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 4;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_SAD_TURN:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, TRUE)));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            task->rWaitTimer = 24;
            task->rMovementStep = 2;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_SLEEPY_SWAY:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, TRUE)));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            task->rWaitTimer = 18;
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, FALSE)));
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            task->rWaitTimer = 18;
            task->rMovementStep = 4;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_WEAK_PAUSE:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            task->rWaitTimer = 24;
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            ObjectEventSetHeldMovement(
                follower,
                GetFaceDirectionMovementAction(GetNinetyDegreeDirection(towardPlayer, TRUE)));
            task->rMovementStep = 3;
            return FALSE;
        default:
            return TRUE;
        }

    case GY_PARTNER_MOVEMENT_STARTLED_HOP:
        switch (task->rMovementStep)
        {
        case 0:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 1;
            return FALSE;
        case 1:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(towardPlayer));
            task->rMovementStep = 2;
            return FALSE;
        case 2:
            task->rWaitTimer = 4;
            task->rMovementStep = 3;
            return FALSE;
        case 3:
            ObjectEventSetHeldMovement(follower, GetJumpInPlaceMovementAction(awayFromPlayer));
            task->rMovementStep = 4;
            return FALSE;
        case 4:
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(towardPlayer));
            task->rMovementStep = 5;
            return FALSE;
        default:
            return TRUE;
        }

    case PARTNER_REACTION_TURN_AWAY_MOVEMENT:
        if (task->rMovementStep == 0)
        {
            ObjectEventSetHeldMovement(follower, GetFaceDirectionMovementAction(awayFromPlayer));
            task->rMovementStep = 1;
            return FALSE;
        }
        return TRUE;
    }

    return TRUE;
}

#undef rReactionId
#undef rCommandIndex
#undef rState
#undef rMode
#undef rWaitTimer
#undef rMovementId
#undef rMovementStep
#undef rInputCooldown
#undef rBubbleEffectId
