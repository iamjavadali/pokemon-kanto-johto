#include "global.h"
#include "bg.h"
#include "event_object_movement.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "script.h"
#include "script_menu.h"
#include "sprite.h"
#include "task.h"
#include "text_window.h"
#include "constants/rgb.h"

#define TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT 0x2A10

// The 6x6 interior plus one border tile on every side gives the 40x40 Yellow
// portrait a 48x48 white field inside a 64x64 visible frame. Moving the window
// origin inward by one tile preserves the accepted portrait screen center.
#define PARTNER_PORTRAIT_WINDOW_X      11
#define PARTNER_PORTRAIT_WINDOW_Y      6
#define PARTNER_PORTRAIT_WINDOW_WIDTH  6
#define PARTNER_PORTRAIT_WINDOW_HEIGHT 6
#define PARTNER_PORTRAIT_SPRITE_OFFSET 36

#define YELLOW_PIKAPIC_PROGRAM_COUNT       29
#define YELLOW_PIKAPIC_SOURCE_TICK_FRAMES  3
#define YELLOW_PIKAPIC_CARRIER_TILE_WIDTH  8
#define YELLOW_PIKAPIC_SOURCE_TILE_WIDTH   5
#define YELLOW_PIKAPIC_CARRIER_SIZE        (64 * 64 / 2)

enum YellowPikaPicTaskMode
{
    YELLOW_PIKAPIC_MODE_SCRIPT,
    YELLOW_PIKAPIC_MODE_DEBUG_BROWSER,
};

struct YellowPikaPicPatchAsset
{
    const u8 *gfx;
    u16 tileCount;
};

struct YellowPikaPicFrame
{
    u8 tilemapId;
    u8 durationTicks;
};

struct YellowPikaPicProgram
{
    const u8 *baseGfx;
    const struct YellowPikaPicPatchAsset *patchAssets;
    const struct YellowPikaPicFrame *frames;
    u16 durationTicks;
    u8 patchAssetCount;
    u8 frameCount;
};

/*
 * Complete portrait graphics for Yellow's directly selectable PikaPic programs
 * 0-28. The PNGs are byte-identical pret/pokeyellow sources; gbagfx promotes
 * their 2bpp grayscale pixels to native GBA 4bpp at build time.
 */
#include "golden_yellow_pikapic_assets.inc"

#define PATCH_ASSET(data_) \
    { .gfx = (data_), .tileCount = sizeof(data_) / TILE_SIZE_4BPP }

#include "golden_yellow_pikapic_data.inc"

static const u16 sPartnerPortraitPalette[16] =
{
    [0]  = RGB_BLACK,
    [1]  = RGB_WHITE,
    [2]  = RGB(21, 21, 21),
    [3]  = RGB(10, 10, 10),
    [4]  = RGB_BLACK,
    [12] = RGB_WHITE,
    [13] = RGB(21, 21, 21),
    [14] = RGB(10, 10, 10),
    [15] = RGB_BLACK,
};

// Dedicated Partner UI palette for FRLG frame 1. Palette index 0 remains the
// transparent border color; the visible purples are remapped to Pikachu yellow
// with darker gold edging while the portrait's white interior stays separate.
static const u16 sPartnerPortraitFramePalette[16] =
{
    [0]  = RGB(12, 24, 12),
    [1]  = RGB(5, 6, 6),
    [2]  = RGB(15, 11, 1),
    [3]  = RGB(30, 25, 4),
    [4]  = RGB(24, 18, 1),
    [5]  = RGB(27, 22, 2),
    [6]  = RGB(31, 28, 6),
    [7]  = RGB(31, 29, 18),
    [8]  = RGB(31, 30, 26),
    [9]  = RGB_BLACK,
    [10] = RGB_BLACK,
    [11] = RGB_BLACK,
    [12] = RGB_BLACK,
    [13] = RGB_BLACK,
    [14] = RGB_WHITE,
    [15] = RGB(10, 8, 3),
};

static EWRAM_DATA u8 sPikaPicBaseCarrier[YELLOW_PIKAPIC_CARRIER_SIZE];
static EWRAM_DATA u8 sPikaPicFrameCarrier[YELLOW_PIKAPIC_CARRIER_SIZE];

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
    { .data = sPikaPicFrameCarrier, .size = sizeof(sPikaPicFrameCarrier) },
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
    .anims = gDummySpriteAnimTable,
    .images = sPartnerPortraitImages,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

#define tSpriteId        data[0]
#define tWindowId        data[1]
#define tProgramId       data[2]
#define tProgramTicks    data[3]
#define tFrameIndex      data[4]
#define tFrameTicksLeft  data[5]
#define tSubframe        data[6]
#define tMode            data[7]

static void Task_PartnerPortrait(u8 taskId);
static void ClosePartnerPortrait(u8 taskId);
static bool8 GoldenYellow_WaitForPartnerPikachuPortrait(void);
static bool32 StartPartnerPortrait(u8 programId, enum YellowPikaPicTaskMode mode, struct ScriptContext *ctx);
static void ResetPartnerPortraitProgram(u8 taskId, u8 programId);
static void RenderPartnerPortraitFrame(u8 taskId);
static void BuildPartnerPortraitBase(u8 programId);
static const u8 *GetPartnerPortraitPatchTile(const struct YellowPikaPicProgram *program, u16 tileIndex);
static bool32 GetPartnerPortraitTilemapSource(u8 tilemapId, u8 row, u8 col, u16 *sourceIndex);

static void BuildPartnerPortraitBase(u8 programId)
{
    const struct YellowPikaPicProgram *program = &sPikaPrograms[programId];
    u8 row;
    u8 col;

    memset(sPikaPicBaseCarrier, 0, sizeof(sPikaPicBaseCarrier));

    // The old accepted carrier centered the 40x40 Yellow art at pixel (12,12).
    // Raw Yellow tiles are placed at tile (1,1), then the 64x64 OBJ itself is
    // shifted +4,+4 pixels so the visible artwork remains at the exact same
    // accepted screen coordinates.
    for (row = 0; row < YELLOW_PIKAPIC_SOURCE_TILE_WIDTH; row++)
    {
        for (col = 0; col < YELLOW_PIKAPIC_SOURCE_TILE_WIDTH; col++)
        {
            u16 srcTile = row * YELLOW_PIKAPIC_SOURCE_TILE_WIDTH + col;
            u16 dstTile = (row + 1) * YELLOW_PIKAPIC_CARRIER_TILE_WIDTH + (col + 1);

            memcpy(
                &sPikaPicBaseCarrier[dstTile * TILE_SIZE_4BPP],
                &program->baseGfx[srcTile * TILE_SIZE_4BPP],
                TILE_SIZE_4BPP);
        }
    }
}

static const u8 *GetPartnerPortraitPatchTile(const struct YellowPikaPicProgram *program, u16 tileIndex)
{
    u8 i;

    for (i = 0; i < program->patchAssetCount; i++)
    {
        if (tileIndex < program->patchAssets[i].tileCount)
            return &program->patchAssets[i].gfx[tileIndex * TILE_SIZE_4BPP];

        tileIndex -= program->patchAssets[i].tileCount;
    }

    return NULL;
}

static bool32 GetPartnerPortraitTilemapSource(u8 tilemapId, u8 row, u8 col, u16 *sourceIndex)
{
    if (tilemapId == 0)
        return FALSE;

    // Yellow tilemaps 9 and 25-39 are the same full 5x5 replacement map.
    if (tilemapId == 9 || (tilemapId >= 25 && tilemapId <= 39))
    {
        *sourceIndex = row * 5 + col;
        return TRUE;
    }

    switch (tilemapId)
    {
    case 10:
        *sourceIndex = 25 + row * 5 + col;
        return TRUE;
    case 11:
        *sourceIndex = 50 + row * 5 + col;
        return TRUE;
    case 12:
        *sourceIndex = 75 + row * 5 + col;
        return TRUE;
    case 14:
        if (row == 2)
        {
            *sourceIndex = col;
            return TRUE;
        }
        break;
    case 15:
        if (row >= 3)
        {
            *sourceIndex = (row - 3) * 5 + col;
            return TRUE;
        }
        break;
    case 16:
        if (row < 3 && col < 2)
        {
            *sourceIndex = row * 2 + col;
            return TRUE;
        }
        break;
    case 17:
        if (row >= 1)
        {
            *sourceIndex = (row - 1) * 5 + col;
            return TRUE;
        }
        break;
    case 18:
        if (row >= 1 && row <= 2 && col >= 3)
        {
            *sourceIndex = (row - 1) * 2 + (col - 3);
            return TRUE;
        }
        break;
    case 19:
        if (row >= 2 && row <= 3 && col < 2)
        {
            *sourceIndex = (row - 2) * 2 + col;
            return TRUE;
        }
        break;
    case 20:
    case 23:
        *sourceIndex = row * 5 + col;
        return TRUE;
    case 21:
        if (row >= 2 && row <= 3)
        {
            *sourceIndex = (row - 2) * 5 + col;
            return TRUE;
        }
        break;
    case 22:
        if (row >= 2 && col < 2)
        {
            *sourceIndex = (row - 2) * 2 + col;
            return TRUE;
        }
        break;
    case 24:
        *sourceIndex = 25 + row * 5 + col;
        return TRUE;
    }

    return FALSE;
}

static void RenderPartnerPortraitFrame(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    const struct YellowPikaPicProgram *program = &sPikaPrograms[task->tProgramId];
    const struct YellowPikaPicFrame *frame = &program->frames[task->tFrameIndex];
    u8 row;
    u8 col;

    memcpy(sPikaPicFrameCarrier, sPikaPicBaseCarrier, sizeof(sPikaPicFrameCarrier));

    for (row = 0; row < YELLOW_PIKAPIC_SOURCE_TILE_WIDTH; row++)
    {
        for (col = 0; col < YELLOW_PIKAPIC_SOURCE_TILE_WIDTH; col++)
        {
            u16 sourceIndex;
            const u8 *sourceTile;

            if (!GetPartnerPortraitTilemapSource(frame->tilemapId, row, col, &sourceIndex))
                continue;

            sourceTile = GetPartnerPortraitPatchTile(program, sourceIndex);
            if (sourceTile != NULL)
            {
                u16 dstTile = (row + 1) * YELLOW_PIKAPIC_CARRIER_TILE_WIDTH + (col + 1);

                memcpy(
                    &sPikaPicFrameCarrier[dstTile * TILE_SIZE_4BPP],
                    sourceTile,
                    TILE_SIZE_4BPP);
            }
        }
    }

    RequestSpriteCopy(
        sPikaPicFrameCarrier,
        (u8 *)(OBJ_VRAM0 + gSprites[task->tSpriteId].oam.tileNum * TILE_SIZE_4BPP),
        sizeof(sPikaPicFrameCarrier));
}

static void ResetPartnerPortraitProgram(u8 taskId, u8 programId)
{
    struct Task *task = &gTasks[taskId];
    const struct YellowPikaPicProgram *program = &sPikaPrograms[programId];

    task->tProgramId = programId;
    task->tProgramTicks = program->durationTicks;
    task->tFrameIndex = 0;
    task->tFrameTicksLeft = program->frames[0].durationTicks;
    task->tSubframe = 0;

    BuildPartnerPortraitBase(programId);
    RenderPartnerPortraitFrame(taskId);
}

static bool32 StartPartnerPortrait(u8 programId, enum YellowPikaPicTaskMode mode, struct ScriptContext *ctx)
{
    u8 spriteId;
    u8 taskId;
    u8 windowId;

    if (programId >= YELLOW_PIKAPIC_PROGRAM_COUNT
     || FindTaskIdByFunc(Task_PartnerPortrait) != TASK_NONE)
        return FALSE;

    // CreateTask can return task 0 when full, which is also a valid task ID.
    // Preflight the task table so resource pressure can never overwrite a task.
    for (taskId = 0; taskId < NUM_TASKS; taskId++)
    {
        if (!gTasks[taskId].isActive)
            break;
    }
    if (taskId == NUM_TASKS)
        return FALSE;

    BuildPartnerPortraitBase(programId);
    memcpy(sPikaPicFrameCarrier, sPikaPicBaseCarrier, sizeof(sPikaPicFrameCarrier));

    LoadSpritePalette(&sPartnerPortraitSpritePalette);

    spriteId = CreateSpriteUnchecked(
        &sPartnerPortraitTemplate,
        PARTNER_PORTRAIT_WINDOW_X * 8 + PARTNER_PORTRAIT_SPRITE_OFFSET,
        PARTNER_PORTRAIT_WINDOW_Y * 8 + PARTNER_PORTRAIT_SPRITE_OFFSET,
        0);
    if (spriteId == MAX_SPRITES)
    {
        FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT);
        return FALSE;
    }

    taskId = CreateTask(Task_PartnerPortrait, 0x50);
    windowId = CreateWindowFromRect(
        PARTNER_PORTRAIT_WINDOW_X,
        PARTNER_PORTRAIT_WINDOW_Y,
        PARTNER_PORTRAIT_WINDOW_WIDTH,
        PARTNER_PORTRAIT_WINDOW_HEIGHT);

    gTasks[taskId].tSpriteId = spriteId;
    gTasks[taskId].tWindowId = windowId;
    gTasks[taskId].tMode = mode;

    gSprites[spriteId].oam.priority = 0;
    ResetPartnerPortraitProgram(taskId, programId);

    // Give Partner Pikachu a dedicated frame without mutating the user's
    // configured menu frame. FRLG frame 1 supplies the geometry; this feature
    // temporarily replaces only its BG palette with the Pikachu yellow theme.
    LoadWindowGfx(windowId, 0, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
    LoadPalette(sPartnerPortraitFramePalette, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM), PLTT_SIZE_4BPP);
    SetStandardWindowBorderStyle(windowId, TRUE);
    ScheduleBgCopyTilemapToVram(0);

    if (mode == YELLOW_PIKAPIC_MODE_SCRIPT)
    {
        SetupNativeScript(ctx, GoldenYellow_WaitForPartnerPikachuPortrait);
        ctx->waitAfterCallNative = TRUE;
    }

    return TRUE;
}

void GoldenYellow_ShowPartnerPikachuPortraitProgram(struct ScriptContext *ctx)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    StartPartnerPortrait(gSpecialVar_0x8004, YELLOW_PIKAPIC_MODE_SCRIPT, ctx);
}

void GoldenYellow_ShowPartnerPikachuPortrait(struct ScriptContext *ctx)
{
    u8 programId;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);

    /*
     * Compatibility adapter for the already-accepted Bill scripts. The Bill
     * vertical slice originally used local IDs 0/1; route those to Yellow's
     * canonical source programs 23/27 while the new Program entry point uses
     * raw source IDs 0-28 for every future reaction.
     */
    if (gSpecialVar_0x8004 == 0)
        programId = 23;
    else if (gSpecialVar_0x8004 == 1)
        programId = 27;
    else
        programId = gSpecialVar_0x8004;

    StartPartnerPortrait(programId, YELLOW_PIKAPIC_MODE_SCRIPT, ctx);
}

bool32 GoldenYellow_DebugStartPikachuPortraitBrowser(void)
{
    // Debug browser controls:
    //   LEFT/RIGHT  previous/next source program (0-28)
    //   A           replay current program
    //   B           close browser
    // Each program otherwise loops after its canonical Yellow outer duration.
    return StartPartnerPortrait(0, YELLOW_PIKAPIC_MODE_DEBUG_BROWSER, NULL);
}

static void Task_PartnerPortrait(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    const struct YellowPikaPicProgram *program;

    if (task->tMode == YELLOW_PIKAPIC_MODE_DEBUG_BROWSER)
    {
        if (gMain.newKeys & B_BUTTON)
        {
            ClosePartnerPortrait(taskId);
            UnlockPlayerFieldControls();
            UnfreezeObjectEvents();
            ScriptContext_Enable();
            return;
        }

        if (gMain.newKeys & DPAD_RIGHT)
        {
            ResetPartnerPortraitProgram(taskId, (task->tProgramId + 1) % YELLOW_PIKAPIC_PROGRAM_COUNT);
            return;
        }

        if (gMain.newKeys & DPAD_LEFT)
        {
            u8 programId = task->tProgramId == 0
                         ? YELLOW_PIKAPIC_PROGRAM_COUNT - 1
                         : task->tProgramId - 1;
            ResetPartnerPortraitProgram(taskId, programId);
            return;
        }

        if (gMain.newKeys & A_BUTTON)
        {
            ResetPartnerPortraitProgram(taskId, task->tProgramId);
            return;
        }
    }
    else if (gMain.newKeys & (A_BUTTON | B_BUTTON))
    {
        ClosePartnerPortrait(taskId);
        return;
    }

    task->tSubframe++;
    if (task->tSubframe < YELLOW_PIKAPIC_SOURCE_TICK_FRAMES)
        return;

    task->tSubframe = 0;

    if (task->tProgramTicks > 0)
        task->tProgramTicks--;

    if (task->tProgramTicks == 0)
    {
        if (task->tMode == YELLOW_PIKAPIC_MODE_DEBUG_BROWSER)
            ResetPartnerPortraitProgram(taskId, task->tProgramId);
        else
            ClosePartnerPortrait(taskId);
        return;
    }

    program = &sPikaPrograms[task->tProgramId];

    if (task->tFrameTicksLeft > 0)
        task->tFrameTicksLeft--;

    if (task->tFrameTicksLeft == 0)
    {
        task->tFrameIndex++;
        if (task->tFrameIndex >= program->frameCount)
            task->tFrameIndex = 0;

        task->tFrameTicksLeft = program->frames[task->tFrameIndex].durationTicks;
        RenderPartnerPortraitFrame(taskId);
    }
}

static void ClosePartnerPortrait(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    DestroySprite(&gSprites[task->tSpriteId]);
    FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_PARTNER_PORTRAIT);

    // Restore the player's selected window-frame tiles and palette before the
    // temporary Partner portrait window is removed, so no later menu inherits
    // the Pikachu-yellow styling.
    LoadUserWindowBorderGfx(
        task->tWindowId,
        STD_WINDOW_BASE_TILE_NUM,
        BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
    ClearToTransparentAndRemoveWindow(task->tWindowId);
    ScheduleBgCopyTilemapToVram(0);
    DestroyTask(taskId);
}

static bool8 GoldenYellow_WaitForPartnerPikachuPortrait(void)
{
    return FindTaskIdByFunc(Task_PartnerPortrait) == TASK_NONE;
}

#undef PATCH_ASSET
#undef tSpriteId
#undef tWindowId
#undef tProgramId
#undef tProgramTicks
#undef tFrameIndex
#undef tFrameTicksLeft
#undef tSubframe
#undef tMode
