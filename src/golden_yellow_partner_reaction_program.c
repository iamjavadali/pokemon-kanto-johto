#include "global.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "golden_yellow_partner_reaction.h"
#include "main.h"
#include "palette.h"
#include "pokemon.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/event_object_movement.h"
#include "constants/field_effects.h"
#include "constants/golden_yellow_partner_reactions.h"
#include "constants/species.h"

#define TAG_GOLDEN_YELLOW_PARTNER_BUBBLE 0x2A11
#define PARTNER_REACTION_BUBBLE_FRAMES   60
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

/*
 * These Yellow bubbles follow the exact sprite lifecycle proven by
 * golden_yellow_sleep_emote.c: source PNG graphics + source PNG palette,
 * a real timed animation, field-coordinate offsets, and stable follower
 * object-event anchoring. Heart, exclamation, and question retain the native
 * GBA field effects because those render correctly in the accepted field UI.
 */
static const u8 sPartnerBubbleHappyGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_happy.png", ".4bpp");
static const u16 sPartnerBubbleHappyPal[] =
    INCGFX_U16("graphics/field_effects/pics/emotion_happy.png", ".gbapal");
static const u8 sPartnerBubbleSkullGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_skull.png", ".4bpp");
static const u16 sPartnerBubbleSkullPal[] =
    INCGFX_U16("graphics/field_effects/pics/emotion_skull.png", ".gbapal");
static const u8 sPartnerBubbleZzzGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_zzz.png", ".4bpp");
static const u16 sPartnerBubbleZzzPal[] =
    INCGFX_U16("graphics/field_effects/pics/emotion_zzz.png", ".gbapal");
static const u8 sPartnerBubbleBoltGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_bolt.png", ".4bpp");
static const u16 sPartnerBubbleBoltPal[] =
    INCGFX_U16("graphics/field_effects/pics/emotion_bolt.png", ".gbapal");
static const u8 sPartnerBubbleFishGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_fish.png", ".4bpp");
static const u16 sPartnerBubbleFishPal[] =
    INCGFX_U16("graphics/field_effects/pics/emotion_fish.png", ".gbapal");

static const struct OamData sPartnerReactionBubbleOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sPartnerReactionBubbleAnim[] =
{
    ANIMCMD_FRAME(0, PARTNER_REACTION_BUBBLE_FRAMES),
    ANIMCMD_END,
};

static const union AnimCmd *const sPartnerReactionBubbleAnims[] =
{
    sPartnerReactionBubbleAnim,
};

#define PARTNER_BUBBLE_IMAGE_TABLE(name_, gfx_) \
    static const struct SpriteFrameImage name_[] = \
    { \
        { .data = (gfx_), .size = sizeof(gfx_) }, \
    }

PARTNER_BUBBLE_IMAGE_TABLE(sPartnerBubbleHappyImages, sPartnerBubbleHappyGfx);
PARTNER_BUBBLE_IMAGE_TABLE(sPartnerBubbleSkullImages, sPartnerBubbleSkullGfx);
PARTNER_BUBBLE_IMAGE_TABLE(sPartnerBubbleZzzImages, sPartnerBubbleZzzGfx);
PARTNER_BUBBLE_IMAGE_TABLE(sPartnerBubbleBoltImages, sPartnerBubbleBoltGfx);
PARTNER_BUBBLE_IMAGE_TABLE(sPartnerBubbleFishImages, sPartnerBubbleFishGfx);

#undef PARTNER_BUBBLE_IMAGE_TABLE

#define PARTNER_BUBBLE_PALETTE(name_, pal_) \
    static const struct SpritePalette name_ = \
    { \
        .data = (pal_), \
        .tag = TAG_GOLDEN_YELLOW_PARTNER_BUBBLE, \
    }

PARTNER_BUBBLE_PALETTE(sPartnerBubbleHappyPalette, sPartnerBubbleHappyPal);
PARTNER_BUBBLE_PALETTE(sPartnerBubbleSkullPalette, sPartnerBubbleSkullPal);
PARTNER_BUBBLE_PALETTE(sPartnerBubbleZzzPalette, sPartnerBubbleZzzPal);
PARTNER_BUBBLE_PALETTE(sPartnerBubbleBoltPalette, sPartnerBubbleBoltPal);
PARTNER_BUBBLE_PALETTE(sPartnerBubbleFishPalette, sPartnerBubbleFishPal);

#undef PARTNER_BUBBLE_PALETTE

static void SpriteCB_PartnerReactionBubble(struct Sprite *sprite);

#define PARTNER_BUBBLE_TEMPLATE(name_, images_) \
    static const struct SpriteTemplate name_ = \
    { \
        .tileTag = TAG_NONE, \
        .paletteTag = TAG_GOLDEN_YELLOW_PARTNER_BUBBLE, \
        .oam = &sPartnerReactionBubbleOam, \
        .anims = sPartnerReactionBubbleAnims, \
        .images = (images_), \
        .affineAnims = gDummySpriteAffineAnimTable, \
        .callback = SpriteCB_PartnerReactionBubble, \
    }

PARTNER_BUBBLE_TEMPLATE(sPartnerBubbleHappyTemplate, sPartnerBubbleHappyImages);
PARTNER_BUBBLE_TEMPLATE(sPartnerBubbleSkullTemplate, sPartnerBubbleSkullImages);
PARTNER_BUBBLE_TEMPLATE(sPartnerBubbleZzzTemplate, sPartnerBubbleZzzImages);
PARTNER_BUBBLE_TEMPLATE(sPartnerBubbleBoltTemplate, sPartnerBubbleBoltImages);
PARTNER_BUBBLE_TEMPLATE(sPartnerBubbleFishTemplate, sPartnerBubbleFishImages);

#undef PARTNER_BUBBLE_TEMPLATE

#define rReactionId       data[0]
#define rCommandIndex     data[1]
#define rState            data[2]
#define rMode             data[3]
#define rWaitTimer        data[4]
#define rMovementId       data[5]
#define rMovementStep     data[6]
#define rInputCooldown    data[7]
#define rBubbleSpriteId   data[8]
#define rBubbleEffectId   data[9]

#define sFollowerObjectEventId data[0]
#define sOwnerTaskId            data[1]

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
static u8 StartCustomPartnerReactionBubble(u8 taskId, u8 bubbleId);
static const struct SpriteTemplate *GetCustomPartnerBubbleTemplate(u8 bubbleId);
static const struct SpritePalette *GetCustomPartnerBubblePalette(u8 bubbleId);
static void DestroyPartnerReactionBubbleSprite(struct Sprite *sprite);
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
    gTasks[taskId].rBubbleSpriteId = MAX_SPRITES;
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
    task->rBubbleSpriteId = MAX_SPRITES;
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
    struct Task *task = &gTasks[taskId];
    struct ObjectEvent *follower = GetFollowerObject();

    if (follower != NULL && follower->active)
        ObjectEventClearHeldMovementIfActive(follower);

    if (task->rBubbleSpriteId < MAX_SPRITES && gSprites[task->rBubbleSpriteId].inUse)
        DestroyPartnerReactionBubbleSprite(&gSprites[task->rBubbleSpriteId]);

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

static bool32 StartPartnerReactionBubble(u8 taskId, u8 bubbleId)
{
    struct Task *task = &gTasks[taskId];
    struct ObjectEvent *follower = GetFollowerObject();
    s16 fieldEffectId = PARTNER_REACTION_NO_FIELD_EFFECT;

    if (follower == NULL || !follower->active)
        return FALSE;

    task->rBubbleSpriteId = MAX_SPRITES;
    task->rBubbleEffectId = PARTNER_REACTION_NO_FIELD_EFFECT;

    switch (bubbleId)
    {
    case GY_PARTNER_BUBBLE_HEART:
        fieldEffectId = FLDEFF_HEART_ICON;
        break;
    case GY_PARTNER_BUBBLE_EXCLAMATION:
        fieldEffectId = FLDEFF_EXCLAMATION_MARK_ICON;
        break;
    case GY_PARTNER_BUBBLE_QUESTION:
        fieldEffectId = FLDEFF_QUESTION_MARK_ICON;
        break;
    case GY_PARTNER_BUBBLE_SMILE:
    case GY_PARTNER_BUBBLE_SKULL:
    case GY_PARTNER_BUBBLE_ZZZ:
    case GY_PARTNER_BUBBLE_BOLT:
    case GY_PARTNER_BUBBLE_FISH:
        task->rBubbleSpriteId = StartCustomPartnerReactionBubble(taskId, bubbleId);
        return task->rBubbleSpriteId < MAX_SPRITES;
    default:
        return FALSE;
    }

    gFieldEffectArguments[0] = follower->localId;
    gFieldEffectArguments[1] = gSaveBlock1Ptr->location.mapNum;
    gFieldEffectArguments[2] = gSaveBlock1Ptr->location.mapGroup;
    gFieldEffectArguments[7] = -1;
    FieldEffectStart(fieldEffectId);
    task->rBubbleEffectId = fieldEffectId;
    return TRUE;
}

static bool32 IsPartnerReactionBubbleActive(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->rBubbleEffectId != PARTNER_REACTION_NO_FIELD_EFFECT)
        return FieldEffectActiveListContains(task->rBubbleEffectId);

    if (task->rBubbleSpriteId < MAX_SPRITES)
        return gSprites[task->rBubbleSpriteId].inUse;

    return FALSE;
}

static const struct SpriteTemplate *GetCustomPartnerBubbleTemplate(u8 bubbleId)
{
    switch (bubbleId)
    {
    case GY_PARTNER_BUBBLE_SMILE:
        return &sPartnerBubbleHappyTemplate;
    case GY_PARTNER_BUBBLE_SKULL:
        return &sPartnerBubbleSkullTemplate;
    case GY_PARTNER_BUBBLE_ZZZ:
        return &sPartnerBubbleZzzTemplate;
    case GY_PARTNER_BUBBLE_BOLT:
        return &sPartnerBubbleBoltTemplate;
    case GY_PARTNER_BUBBLE_FISH:
        return &sPartnerBubbleFishTemplate;
    default:
        return NULL;
    }
}

static const struct SpritePalette *GetCustomPartnerBubblePalette(u8 bubbleId)
{
    switch (bubbleId)
    {
    case GY_PARTNER_BUBBLE_SMILE:
        return &sPartnerBubbleHappyPalette;
    case GY_PARTNER_BUBBLE_SKULL:
        return &sPartnerBubbleSkullPalette;
    case GY_PARTNER_BUBBLE_ZZZ:
        return &sPartnerBubbleZzzPalette;
    case GY_PARTNER_BUBBLE_BOLT:
        return &sPartnerBubbleBoltPalette;
    case GY_PARTNER_BUBBLE_FISH:
        return &sPartnerBubbleFishPalette;
    default:
        return NULL;
    }
}

static u8 StartCustomPartnerReactionBubble(u8 taskId, u8 bubbleId)
{
    const struct SpriteTemplate *template = GetCustomPartnerBubbleTemplate(bubbleId);
    const struct SpritePalette *palette = GetCustomPartnerBubblePalette(bubbleId);
    struct ObjectEvent *follower = GetFollowerObject();
    u8 spriteId;

    if (template == NULL || palette == NULL || follower == NULL || !follower->active)
        return MAX_SPRITES;

    LoadSpritePalette(palette);
    spriteId = CreateSprite(template, 0, 0, 0x52);
    if (spriteId == MAX_SPRITES)
    {
        FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_PARTNER_BUBBLE);
        return MAX_SPRITES;
    }

    gSprites[spriteId].coordOffsetEnabled = TRUE;
    gSprites[spriteId].sFollowerObjectEventId = follower - gObjectEvents;
    gSprites[spriteId].sOwnerTaskId = taskId;
    SpriteCB_PartnerReactionBubble(&gSprites[spriteId]);
    return spriteId;
}

static void SpriteCB_PartnerReactionBubble(struct Sprite *sprite)
{
    u8 objectEventId = sprite->sFollowerObjectEventId;
    u8 taskId = sprite->sOwnerTaskId;

    if (objectEventId >= OBJECT_EVENTS_COUNT
     || !gObjectEvents[objectEventId].active
     || taskId >= NUM_TASKS
     || !gTasks[taskId].isActive
     || gTasks[taskId].func != Task_PartnerReaction
     || sprite->animEnded)
    {
        DestroyPartnerReactionBubbleSprite(sprite);
        return;
    }

    {
        struct Sprite *followerSprite = &gSprites[gObjectEvents[objectEventId].spriteId];

        sprite->x = followerSprite->x;
        sprite->y = followerSprite->y - 16;
        sprite->x2 = followerSprite->x2;
        sprite->y2 = followerSprite->y2;
    }
}

static void DestroyPartnerReactionBubbleSprite(struct Sprite *sprite)
{
    u8 taskId = sprite->sOwnerTaskId;

    if (taskId < NUM_TASKS
     && gTasks[taskId].isActive
     && gTasks[taskId].func == Task_PartnerReaction)
        gTasks[taskId].rBubbleSpriteId = MAX_SPRITES;

    DestroySprite(sprite);
    FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_PARTNER_BUBBLE);
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
#undef rBubbleSpriteId
#undef rBubbleEffectId
#undef sFollowerObjectEventId
#undef sOwnerTaskId
