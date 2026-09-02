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
#include "script_movement.h"
#include "sound.h"
#include "task.h"
#include "constants/event_object_movement.h"
#include "constants/field_effects.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/species.h"

#define PARTNER_REACTION_NO_FIELD_EFFECT (-1)
#define PARTNER_REACTION_TURN_AWAY_MOVEMENT 0xFE
#define PARTNER_REACTION_POSE_HOLD_FRAMES 6
#define PARTNER_REACTION_FIELD_TALK_POSE_HOLD_FRAMES 24

// Authored maps such as Bill's Sea Cottage can intentionally suppress the
// generic overworld follower and render canonical Partner Pikachu as a normal
// scene-owned object. P7 integration must still use the single P3 director, so
// keep a narrowly scoped target override rather than duplicating reaction logic.
static bool8 sPartnerReactionObjectOverrideActive;
static u8 sPartnerReactionObjectEventId;

void GoldenYellow_SetPartnerPikachuReactionObject(u8 objectEventId)
{
    if (objectEventId >= OBJECT_EVENTS_COUNT || !gObjectEvents[objectEventId].active)
    {
        GoldenYellow_ClearPartnerPikachuReactionObject();
        return;
    }

    sPartnerReactionObjectEventId = objectEventId;
    sPartnerReactionObjectOverrideActive = TRUE;
}

void GoldenYellow_ClearPartnerPikachuReactionObject(void)
{
    sPartnerReactionObjectOverrideActive = FALSE;
    sPartnerReactionObjectEventId = 0;
}

static struct ObjectEvent *GoldenYellow_GetPartnerReactionObject(void)
{
    if (sPartnerReactionObjectOverrideActive
     && sPartnerReactionObjectEventId < OBJECT_EVENTS_COUNT
     && gObjectEvents[sPartnerReactionObjectEventId].active)
        return &gObjectEvents[sPartnerReactionObjectEventId];

    return GetFollowerObject();
}

// From this point forward, all existing P3 object operations use the selected
// visible Partner object. With no authored override this resolves exactly to
// GetFollowerObject(), preserving the accepted P3/P4/P7A behavior unchanged.
#define GetFollowerObject() GoldenYellow_GetPartnerReactionObject()

enum GoldenYellowPartnerReactionTaskMode
{
    GY_PARTNER_REACTION_MODE_ONESHOT,
    GY_PARTNER_REACTION_MODE_FIELD_TALK,
    GY_PARTNER_REACTION_MODE_DEBUG_BROWSER,
};

enum GoldenYellowPartnerReactionTaskState
{
    GY_PARTNER_REACTION_STATE_RUN_COMMAND,
    GY_PARTNER_REACTION_STATE_WAIT_CRY,
    GY_PARTNER_REACTION_STATE_WAIT_BUBBLE,
    GY_PARTNER_REACTION_STATE_WAIT_MOVEMENT,
    GY_PARTNER_REACTION_STATE_WAIT_POSE,
    GY_PARTNER_REACTION_STATE_DEBUG_WAIT_PORTRAIT,
    GY_PARTNER_REACTION_STATE_WAIT_PORTRAIT,
    GY_PARTNER_REACTION_STATE_WAIT_DELAY,
    GY_PARTNER_REACTION_STATE_BROWSER_IDLE,
};

enum GoldenYellowPartnerPortraitPose
{
    GY_PARTNER_POSE_KEEP,
    GY_PARTNER_POSE_FACE_PLAYER,
    GY_PARTNER_POSE_FACE_AWAY,
    GY_PARTNER_POSE_SIDE_CLOCKWISE,
    GY_PARTNER_POSE_SIDE_COUNTERCLOCKWISE,
};

struct GoldenYellowPartnerReactionCommand
{
    u8 type;
    u8 arg;
    u16 value;
};

#include "golden_yellow_partner_reaction_data.inc"

// Proven follower movement scripts from data/scripts/follower.inc. P3 invokes
// these through the same ScriptMovement executor used by applymovement rather
// than feeding individual held movements directly into the follower object.
extern const u8 FollowerSkippingMovement[];
extern const u8 FollowerShiverVerticalMovement[];
extern const u8 FollowerShiverHorizontalMovement[];
extern const u8 FollowerLookAway[];
extern const u8 FollowerLookAwayBark[];
extern const u8 FollowerLookAwayPokeG[];
extern const u8 FollowerPokeGround[];
extern const u8 FollowerHopFast[];
extern const u8 FollowerDizzy[];
extern const u8 FollowerLookAround[];
extern const u8 FollowerDance[];
extern const u8 FollowerStartled[];

// A few Yellow-specific body-language beats do not have an exact stock
// follower equivalent. They still run through ScriptMovement, remain entirely
// in-place, and use the same movement actions as the existing follower scripts.
static const u8 sPartnerEnergeticMovement[] =
{
    MOVEMENT_ACTION_FACE_PLAYER,
    MOVEMENT_ACTION_LOCK_FACING_DIRECTION,
    MOVEMENT_ACTION_JUMP_IN_PLACE_DOWN,
    MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_DOWN,
    MOVEMENT_ACTION_WALK_IN_PLACE_FAST_DOWN,
    MOVEMENT_ACTION_JUMP_IN_PLACE_DOWN,
    MOVEMENT_ACTION_UNLOCK_FACING_DIRECTION,
    MOVEMENT_ACTION_FACE_PLAYER,
    MOVEMENT_ACTION_STEP_END,
};

static const u8 sPartnerStrongAffectionMovement[] =
{
    MOVEMENT_ACTION_FACE_PLAYER,
    MOVEMENT_ACTION_LOCK_FACING_DIRECTION,
    MOVEMENT_ACTION_JUMP_IN_PLACE_DOWN,
    MOVEMENT_ACTION_DELAY_4,
    MOVEMENT_ACTION_JUMP_IN_PLACE_DOWN,
    MOVEMENT_ACTION_WALK_IN_PLACE_FAST_DOWN,
    MOVEMENT_ACTION_UNLOCK_FACING_DIRECTION,
    MOVEMENT_ACTION_FACE_PLAYER,
    MOVEMENT_ACTION_STEP_END,
};

static const u8 sPartnerRefusalMovement[] =
{
    MOVEMENT_ACTION_FACE_LEFT,
    MOVEMENT_ACTION_DELAY_4,
    MOVEMENT_ACTION_FACE_RIGHT,
    MOVEMENT_ACTION_DELAY_4,
    MOVEMENT_ACTION_FACE_LEFT,
    MOVEMENT_ACTION_DELAY_4,
    MOVEMENT_ACTION_FACE_RIGHT,
    MOVEMENT_ACTION_DELAY_4,
    MOVEMENT_ACTION_FACE_PLAYER,
    MOVEMENT_ACTION_STEP_END,
};

static const u8 sPartnerFearMovement[] =
{
    MOVEMENT_ACTION_FACE_AWAY_PLAYER,
    MOVEMENT_ACTION_LOCK_FACING_DIRECTION,
    MOVEMENT_ACTION_JUMP_IN_PLACE_DOWN,
    MOVEMENT_ACTION_UNLOCK_FACING_DIRECTION,
    MOVEMENT_ACTION_DELAY_8,
    MOVEMENT_ACTION_FACE_LEFT,
    MOVEMENT_ACTION_DELAY_8,
    MOVEMENT_ACTION_FACE_RIGHT,
    MOVEMENT_ACTION_DELAY_8,
    MOVEMENT_ACTION_FACE_PLAYER,
    MOVEMENT_ACTION_STEP_END,
};

static const u8 sPartnerSleepySwayMovement[] =
{
    MOVEMENT_ACTION_FACE_LEFT,
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_FACE_RIGHT,
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_FACE_PLAYER,
    MOVEMENT_ACTION_STEP_END,
};

// Final overworld pose before each portrait. This is deliberately independent
// from the expressive movement preset so a reaction can dance, recoil, sway,
// or hop first and still visually agree with the Yellow close-up pose.
static const u8 sPartnerReactionPortraitPoses[GY_PARTNER_REACTION_COUNT] =
{
    [GY_PARTNER_REACTION_EMPTY]                  = GY_PARTNER_POSE_KEEP,
    [GY_PARTNER_REACTION_NEUTRAL]                = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_HAPPY]                  = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_SUBDUED]                = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_PLAYFUL]                = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_DISPLEASED]             = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_IRRITATED]              = GY_PARTNER_POSE_FACE_AWAY,
    [GY_PARTNER_REACTION_ENERGETIC_HAPPY]        = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_HAPPY_GRIN]             = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_STRONG_DISPLEASURE]     = GY_PARTNER_POSE_FACE_AWAY,
    [GY_PARTNER_REACTION_AFFECTION]              = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_SLEEPING]               = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_FROWNING]               = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_ALOOF]                  = GY_PARTNER_POSE_FACE_AWAY,
    [GY_PARTNER_REACTION_VERY_ANGRY]             = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_CONTENT]                = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_STRONG_HAPPINESS]       = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_UNHAPPY]                = GY_PARTNER_POSE_FACE_AWAY,
    [GY_PARTNER_REACTION_CAPTURE_SUCCESS]        = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_STRONG_AFFECTION]       = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_MAX_AFFECTION]          = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_FISHING]                = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_TOWER_AFRAID]           = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_BILL_CONFUSED]          = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_THUNDER_STONE_REFUSAL]  = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_ELECTRIC_POWER]         = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_PEWTER_JIGGLYPUFF]      = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_BILL_SHOCKED]           = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_STATUS_SICK]            = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_FAN_CLUB_AFFECTION]     = GY_PARTNER_POSE_FACE_PLAYER,
    [GY_PARTNER_REACTION_FAN_CLUB_MAX_AFFECTION] = GY_PARTNER_POSE_FACE_AWAY,
    [GY_PARTNER_REACTION_BILL_POST_STATE]        = GY_PARTNER_POSE_SIDE_CLOCKWISE,
    [GY_PARTNER_REACTION_BILL_INTERMEDIATE]      = GY_PARTNER_POSE_SIDE_CLOCKWISE,
};

#define rReactionId          data[0]
#define rCommandIndex        data[1]
#define rState               data[2]
#define rMode                data[3]
#define rWaitTimer           data[4]
#define rMovementId          data[5]
#define rMovementStep        data[6]
#define rInputCooldown       data[7]
#define rBubbleEffectId      data[8]
#define rPoseApplied         data[9]
#define rDebugPortraitReady  data[10]

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
static const u8 *GetPartnerReactionMovementScript(u8 reactionId, u8 movementId, const struct ObjectEvent *follower);
static void StartPartnerReactionMovement(u8 taskId, u8 movementId);
static bool32 UpdatePartnerReactionMovement(u8 taskId);
static bool32 UpdatePartnerReactionPose(u8 taskId);
static enum Direction GetFollowerDirectionTowardPlayer(const struct ObjectEvent *follower);

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

bool32 GoldenYellow_StartPartnerPikachuFieldTalkReaction(u8 reactionId)
{
    return StartPartnerReactionTask(reactionId, GY_PARTNER_REACTION_MODE_FIELD_TALK);
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
    // During the pre-portrait inspection hold:
    //   A           show the portrait
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
    task->rPoseApplied = FALSE;
    task->rDebugPortraitReady = FALSE;
}

static void FinishPartnerReaction(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->rMode == GY_PARTNER_REACTION_MODE_DEBUG_BROWSER)
    {
        struct ObjectEvent *follower = GetFollowerObject();

        // Debug-browser normalization only: each Emotion must begin from the
        // same player-facing baseline so its body-language choreography can be
        // evaluated independently. Authored one-shot reactions intentionally
        // retain their story/portrait-synchronized final pose.
        if (follower != NULL && follower->active)
        {
            ObjectEventClearHeldMovementIfActive(follower);
            UnfreezeObjectEvent(follower);
            ObjectEventTurn(follower, GetFollowerDirectionTowardPlayer(follower));
        }

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
    {
        ObjectEventClearHeldMovementIfActive(follower);
        UnfreezeObjectEvent(follower);
    }

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

    case GY_PARTNER_REACTION_STATE_WAIT_POSE:
        if (UpdatePartnerReactionPose(taskId))
            task->rState = GY_PARTNER_REACTION_STATE_RUN_COMMAND;
        break;

    case GY_PARTNER_REACTION_STATE_DEBUG_WAIT_PORTRAIT:
        // Debug browser only: hold the final overworld pose indefinitely so
        // the tester can inspect the complete body-language result before the
        // portrait covers Pikachu. A advances; B exits the browser.
        if (gMain.newKeys & B_BUTTON)
        {
            ClosePartnerReactionBrowser(taskId);
            return;
        }

        if (gMain.newKeys & A_BUTTON)
        {
            task->rDebugPortraitReady = TRUE;
            task->rInputCooldown = 2;
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
        // Yellow $FF is true silence. Required semantic IDs use the dedicated
        // PCM table; unavailable IDs advance without synthesizing a fallback.
        if (PlayYellowPikachuCry(command->arg))
            task->rState = GY_PARTNER_REACTION_STATE_WAIT_CRY;
        else
            task->rCommandIndex++;
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
        // The expressive movement may intentionally end in any direction.
        // Resolve a separate pose before opening the close-up so overworld
        // Pikachu and Yellow portrait body language agree with one another.
        if (!task->rPoseApplied)
        {
            task->rPoseApplied = TRUE;
            task->rMovementStep = 0;
            task->rWaitTimer = 0;
            task->rState = GY_PARTNER_REACTION_STATE_WAIT_POSE;
        }
        else if (task->rMode == GY_PARTNER_REACTION_MODE_DEBUG_BROWSER
              && !task->rDebugPortraitReady)
        {
            task->rState = GY_PARTNER_REACTION_STATE_DEBUG_WAIT_PORTRAIT;
        }
        else
        {
            task->rPoseApplied = FALSE;
            task->rDebugPortraitReady = FALSE;
            if (GoldenYellow_StartPartnerPikachuPortraitForReaction(command->arg))
                task->rState = GY_PARTNER_REACTION_STATE_WAIT_PORTRAIT;
            else
                task->rCommandIndex++;
        }
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

static const u8 *GetPartnerReactionMovementScript(u8 reactionId, u8 movementId, const struct ObjectEvent *follower)
{
    enum Direction towardPlayer = GetFollowerDirectionTowardPlayer(follower);

    switch (movementId)
    {
    case GY_PARTNER_MOVEMENT_AGITATED_TURN:
        return FollowerLookAwayPokeG;

    case GY_PARTNER_MOVEMENT_ALOOF_TURN:
        if (reactionId == GY_PARTNER_REACTION_IRRITATED)
            return FollowerLookAwayBark;
        return FollowerLookAway;

    case GY_PARTNER_MOVEMENT_ENERGETIC_HOP:
        return sPartnerEnergeticMovement;

    case GY_PARTNER_MOVEMENT_PLAYFUL_HOP:
        return FollowerHopFast;

    case GY_PARTNER_MOVEMENT_HAPPY_BOUNCE:
        return FollowerSkippingMovement;

    case GY_PARTNER_MOVEMENT_AFFECTION_BOUNCE:
        if (reactionId == GY_PARTNER_REACTION_STRONG_AFFECTION)
            return sPartnerStrongAffectionMovement;
        return FollowerSkippingMovement;

    case GY_PARTNER_MOVEMENT_CELEBRATE:
        return FollowerDance;

    case GY_PARTNER_MOVEMENT_CONFUSED_LOOK:
        return FollowerLookAround;

    case GY_PARTNER_MOVEMENT_REFUSAL_SHAKE:
        return sPartnerRefusalMovement;

    case GY_PARTNER_MOVEMENT_FEAR_RECOIL:
        return sPartnerFearMovement;

    case GY_PARTNER_MOVEMENT_ELECTRIC_JOLT:
        if (towardPlayer == DIR_NORTH || towardPlayer == DIR_SOUTH)
            return FollowerShiverVerticalMovement;
        return FollowerShiverHorizontalMovement;

    case GY_PARTNER_MOVEMENT_SAD_TURN:
        if (reactionId == GY_PARTNER_REACTION_DISPLEASED)
            return FollowerPokeGround;
        return FollowerLookAway;

    case GY_PARTNER_MOVEMENT_SLEEPY_SWAY:
        return sPartnerSleepySwayMovement;

    case GY_PARTNER_MOVEMENT_WEAK_PAUSE:
        return FollowerDizzy;

    case GY_PARTNER_MOVEMENT_STARTLED_HOP:
        return FollowerStartled;

    case PARTNER_REACTION_TURN_AWAY_MOVEMENT:
        return FollowerLookAwayBark;
    }

    return NULL;
}

static bool32 UpdatePartnerReactionPose(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    struct ObjectEvent *follower = GetFollowerObject();
    enum Direction towardPlayer;
    enum Direction poseDirection;
    u8 pose = sPartnerReactionPortraitPoses[task->rReactionId];

    if (follower == NULL || !follower->active || pose == GY_PARTNER_POSE_KEEP)
        return TRUE;

    if (task->rWaitTimer > 0)
    {
        task->rWaitTimer--;
        return FALSE;
    }

    if (task->rMovementStep == 0)
    {
        towardPlayer = GetFollowerDirectionTowardPlayer(follower);
        switch (pose)
        {
        case GY_PARTNER_POSE_FACE_PLAYER:
            poseDirection = towardPlayer;
            break;
        case GY_PARTNER_POSE_FACE_AWAY:
            poseDirection = GetOppositeDirection(towardPlayer);
            break;
        case GY_PARTNER_POSE_SIDE_CLOCKWISE:
            poseDirection = GetNinetyDegreeDirection(towardPlayer, TRUE);
            break;
        case GY_PARTNER_POSE_SIDE_COUNTERCLOCKWISE:
            poseDirection = GetNinetyDegreeDirection(towardPlayer, FALSE);
            break;
        default:
            return TRUE;
        }

        // ScriptMovement freezes an object at STEP_END. The movement wait
        // explicitly unfreezes the Partner, then the final portrait pose uses
        // ObjectEventTurn directly so facing cannot be swallowed by the
        // follower's normal held-movement callback.
        ObjectEventClearHeldMovementIfActive(follower);
        UnfreezeObjectEvent(follower);
        ObjectEventTurn(follower, poseDirection);
        task->rMovementStep = 1;
        task->rWaitTimer = task->rMode == GY_PARTNER_REACTION_MODE_FIELD_TALK
                         ? PARTNER_REACTION_FIELD_TALK_POSE_HOLD_FRAMES
                         : PARTNER_REACTION_POSE_HOLD_FRAMES;
        return FALSE;
    }

    return TRUE;
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
    const u8 *movementScript;

    if (follower == NULL || !follower->active)
        return TRUE;

    movementScript = GetPartnerReactionMovementScript(task->rReactionId, task->rMovementId, follower);
    if (movementScript == NULL)
        return TRUE;

    if (task->rMovementStep == 0)
    {
        // Clear only the follower engine's ordinary held step, then hand the
        // whole choreography to the same executor used by applymovement.
        ObjectEventClearHeldMovementIfActive(follower);
        UnfreezeObjectEvent(follower);

        // ScriptMovement returns FALSE when it successfully schedules a script.
        // Fail open instead of stranding field controls if an authored scene
        // incorrectly overlaps another scripted movement.
        if (ScriptMovement_StartObjectMovementScript(
                follower->localId,
                gSaveBlock1Ptr->location.mapNum,
                gSaveBlock1Ptr->location.mapGroup,
                movementScript))
            return TRUE;

        task->rMovementStep = 1;
        return FALSE;
    }

    if (!ScriptMovement_IsObjectMovementFinished(
            follower->localId,
            gSaveBlock1Ptr->location.mapNum,
            gSaveBlock1Ptr->location.mapGroup))
        return FALSE;

    // STEP_END freezes scripted objects. Restore follower authority before the
    // cry/pose/portrait pipeline continues.
    UnfreezeObjectEvent(follower);
    task->rMovementStep = 0;
    return TRUE;
}

#undef GetFollowerObject
#undef rReactionId
#undef rCommandIndex
#undef rState
#undef rMode
#undef rWaitTimer
#undef rMovementId
#undef rMovementStep
#undef rInputCooldown
#undef rBubbleEffectId
#undef rPoseApplied
#undef rDebugPortraitReady
