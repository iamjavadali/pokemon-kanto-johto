#include "global.h"
#include "bg.h"
#include "event_data.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "script.h"
#include "script_menu.h"
#include "sprite.h"
#include "task.h"
#include "constants/rgb.h"

#define TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT 0x2A10

#define PARTNER_PORTRAIT_WINDOW_X      10
#define PARTNER_PORTRAIT_WINDOW_Y      5
#define PARTNER_PORTRAIT_WINDOW_WIDTH  8
#define PARTNER_PORTRAIT_WINDOW_HEIGHT 8

// Yellow's portrait interpreter advances once per Delay3. Multiplying the
// original script durations by three preserves the visible timing on GBA.
#define BILL_CONFUSED_DURATION_FRAMES  (70 * 3)
#define BILL_SHOCKED_DURATION_FRAMES   (30 * 3)

enum GoldenYellowPartnerPortrait
{
    PARTNER_PORTRAIT_BILL_CONFUSED,
    PARTNER_PORTRAIT_BILL_SHOCKED,
    PARTNER_PORTRAIT_COUNT,
};

// Original Yellow sources, centered unchanged inside a native 64x64 GBA OBJ:
//   confused 0: pret/pokeyellow gfx/pikachu/unknown_e718f.png
//   confused 1: pret/pokeyellow gfx/pikachu/unknown_e731f.png
//   shocked  0: pret/pokeyellow gfx/pikachu/unknown_f0abf.png
//   shocked  1: pret/pokeyellow gfx/pikachu/unknown_f0b64.png
static const u8 sBillConfusedFrame0Gfx[] =
    INCGFX_U8("graphics/golden_yellow/pikachu_portraits/bill_confused_0.png", ".4bpp");
static const u8 sBillConfusedFrame1Gfx[] =
    INCGFX_U8("graphics/golden_yellow/pikachu_portraits/bill_confused_1.png", ".4bpp");
static const u8 sBillShockedFrame0Gfx[] =
    INCGFX_U8("graphics/golden_yellow/pikachu_portraits/bill_shocked_0.png", ".4bpp");
static const u8 sBillShockedFrame1Gfx[] =
    INCGFX_U8("graphics/golden_yellow/pikachu_portraits/bill_shocked_1.png", ".4bpp");

static const u16 sPartnerPortraitPalette[16] =
{
    [0] = RGB_BLACK,       // OBJ-transparent carrier color
    [1] = RGB_WHITE,
    [2] = RGB(21, 21, 21),
    [3] = RGB(10, 10, 10),
    [4] = RGB_BLACK,
};

static const struct OamData sPartnerPortraitOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct SpriteFrameImage sPartnerPortraitImages[] =
{
    {
        .data = sBillConfusedFrame0Gfx,
        .size = sizeof(sBillConfusedFrame0Gfx),
    },
    {
        .data = sBillConfusedFrame1Gfx,
        .size = sizeof(sBillConfusedFrame1Gfx),
    },
    {
        .data = sBillShockedFrame0Gfx,
        .size = sizeof(sBillShockedFrame0Gfx),
    },
    {
        .data = sBillShockedFrame1Gfx,
        .size = sizeof(sBillShockedFrame1Gfx),
    },
};

// PikaPicAnimScript23 draws the first confused/questioning frame, waits
// 16 interpreter ticks, then replaces the full 5x5 portrait with its second
// graphics set. Yellow runs one interpreter tick per Delay3.
static const union AnimCmd sPartnerPortraitAnimBillConfused[] =
{
    ANIMCMD_FRAME(0, 16 * 3),
    ANIMCMD_FRAME(1, 1),
    ANIMCMD_END,
};

// PikaPicAnimScript27 waits four interpreter ticks before replacing its first
// portrait frame with the radiating surprise frame. The task owns the original
// 30-tick total duration, so that second frame remains visible until close.
static const union AnimCmd sPartnerPortraitAnimBillShocked[] =
{
    ANIMCMD_FRAME(2, 4 * 3),
    ANIMCMD_FRAME(3, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sPartnerPortraitAnims[] =
{
    [PARTNER_PORTRAIT_BILL_CONFUSED] = sPartnerPortraitAnimBillConfused,
    [PARTNER_PORTRAIT_BILL_SHOCKED] = sPartnerPortraitAnimBillShocked,
};

static const struct SpritePalette sPartnerPortraitSpritePalette =
{
    .data = sPartnerPortraitPalette,
    .tag = TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT,
};

static const struct SpriteTemplate sPartnerPortraitTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT,
    .oam = &sPartnerPortraitOam,
    .anims = sPartnerPortraitAnims,
    .images = sPartnerPortraitImages,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

#define tSpriteId data[0]
#define tWindowId data[1]
#define tTimer    data[2]

static void Task_PartnerPortrait(u8 taskId);
static void ClosePartnerPortrait(u8 taskId);
static bool8 GoldenYellow_WaitForPartnerPikachuPortrait(void);

void GoldenYellow_ShowPartnerPikachuPortrait(struct ScriptContext *ctx)
{
    u8 spriteId;
    u8 taskId;
    u8 windowId;
    u16 reaction = gSpecialVar_0x8004;
    s16 duration;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    if (reaction >= PARTNER_PORTRAIT_COUNT
     || FindTaskIdByFunc(Task_PartnerPortrait) != TASK_NONE)
        return;

    duration = (reaction == PARTNER_PORTRAIT_BILL_CONFUSED)
             ? BILL_CONFUSED_DURATION_FRAMES
             : BILL_SHOCKED_DURATION_FRAMES;

    LoadSpritePalette(&sPartnerPortraitSpritePalette);

    spriteId = CreateSpriteUnchecked(
        &sPartnerPortraitTemplate,
        PARTNER_PORTRAIT_WINDOW_X * 8 + 40,
        PARTNER_PORTRAIT_WINDOW_Y * 8 + 40,
        0);
    if (spriteId == MAX_SPRITES)
    {
        FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT);
        return;
    }

    // CreateTask returns task 0 when full, which is also a valid task ID. Check
    // capacity first so a resource-starved field scene can safely skip the
    // portrait instead of overwriting an unrelated active task.
    for (taskId = 0; taskId < NUM_TASKS; taskId++)
    {
        if (!gTasks[taskId].isActive)
            break;
    }
    if (taskId == NUM_TASKS)
    {
        DestroySprite(&gSprites[spriteId]);
        FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT);
        return;
    }

    taskId = CreateTask(Task_PartnerPortrait, 0x50);

    windowId = CreateWindowFromRect(
        PARTNER_PORTRAIT_WINDOW_X,
        PARTNER_PORTRAIT_WINDOW_Y,
        PARTNER_PORTRAIT_WINDOW_WIDTH,
        PARTNER_PORTRAIT_WINDOW_HEIGHT);

    gTasks[taskId].tSpriteId = spriteId;
    gTasks[taskId].tWindowId = windowId;
    gTasks[taskId].tTimer = duration;

    StartSpriteAnim(&gSprites[spriteId], reaction);
    gSprites[spriteId].oam.priority = 0;

    SetStandardWindowBorderStyle(windowId, TRUE);
    ScheduleBgCopyTilemapToVram(0);

    // Yield the callnative immediately and keep the script in native mode
    // until the portrait task has completely destroyed its OBJ/window state.
    SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuPortrait);
    ctx->waitAfterCallNative = TRUE;
}

static void Task_PartnerPortrait(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->tTimer > 0)
        task->tTimer--;

    if (task->tTimer == 0
     || (gMain.newKeys & (A_BUTTON | B_BUTTON)))
    {
        ClosePartnerPortrait(taskId);
    }
}

static void ClosePartnerPortrait(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    DestroySprite(&gSprites[task->tSpriteId]);
    FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT);
    ClearToTransparentAndRemoveWindow(task->tWindowId);
    ScheduleBgCopyTilemapToVram(0);
    DestroyTask(taskId);
}

static bool8 GoldenYellow_WaitForPartnerPikachuPortrait(void)
{
    return FindTaskIdByFunc(Task_PartnerPortrait) == TASK_NONE;
}

#undef tSpriteId
#undef tWindowId
#undef tTimer