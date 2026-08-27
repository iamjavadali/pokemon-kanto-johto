#include "global.h"
#include "event_object_movement.h"
#include "palette.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/field_effects.h"
#include "constants/songs.h"

#define GY_CHARM_HEAL_BALL_X 93
#define GY_CHARM_HEAL_BALL_Y 36
#define GY_CHARM_HEAL_MONITOR_X 128
#define GY_CHARM_HEAL_MONITOR_Y 24

static const u32 sGyCharmanderPokeballGlowGfx[] =
    INCGFX_U32("graphics/field_effects/pics/pokeball_glow.png", ".4bpp");
static const u16 sGyCharmanderPokeballGlowPal[16] =
    INCGFX_U16("graphics/field_effects/palettes/pokeball_glow.pal", ".gbapal");
static const u16 sGyCharmanderPokecenterMonitorGfx[] =
    INCGFX_U16("graphics/field_effects/pics/pokecenter_monitor/frlg.png", ".4bpp");

static const struct OamData sGyCharmanderOam8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

static const struct OamData sGyCharmanderOam32x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
};

static const struct SpriteFrameImage sGyCharmanderPokeballGlowImages[] =
{
    obj_frame_tiles(sGyCharmanderPokeballGlowGfx)
};

static const struct SpriteFrameImage sGyCharmanderPokecenterMonitorImages[] =
{
    {sGyCharmanderPokecenterMonitorGfx + 0x000, 0x100},
    {sGyCharmanderPokecenterMonitorGfx + 0x080, 0x100},
    {sGyCharmanderPokecenterMonitorGfx + 0x100, 0x100},
    {sGyCharmanderPokecenterMonitorGfx + 0x180, 0x100},
};

static const union AnimCmd sGyCharmanderAnimStatic[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sGyCharmanderAnimHeal[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(1, 16),
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(1, 16),
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(1, 16),
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(1, 16),
    ANIMCMD_END,
};

static const union AnimCmd *const sGyCharmanderAnimTable[] =
{
    sGyCharmanderAnimStatic,
    sGyCharmanderAnimHeal,
};

static const struct SpritePalette sGyCharmanderHealPalette =
{
    .data = sGyCharmanderPokeballGlowPal,
    .tag = FLDEFF_PAL_TAG_POKEBALL_GLOW,
};

static const struct SpriteTemplate sGyCharmanderBallTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = FLDEFF_PAL_TAG_POKEBALL_GLOW,
    .oam = &sGyCharmanderOam8x8,
    .anims = sGyCharmanderAnimTable,
    .images = sGyCharmanderPokeballGlowImages,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sGyCharmanderMonitorTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = FLDEFF_PAL_TAG_POKEBALL_GLOW,
    .oam = &sGyCharmanderOam32x16,
    .anims = sGyCharmanderAnimTable,
    .images = sGyCharmanderPokecenterMonitorImages,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static u8 sGyCharmanderHealTaskId = TASK_NONE;

#define tState data[0]
#define tTimer data[1]
#define tBallSprite data[2]
#define tMonitorSprite data[3]

static void Task_GoldenYellowCharmanderSingleHeal(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case 0:
        LoadSpritePalette(&sGyCharmanderHealPalette);
        task->tBallSprite = CreateSpriteAtEnd(&sGyCharmanderBallTemplate,
                                               GY_CHARM_HEAL_BALL_X,
                                               GY_CHARM_HEAL_BALL_Y,
                                               0);
        task->tMonitorSprite = CreateSpriteAtEnd(&sGyCharmanderMonitorTemplate,
                                                  GY_CHARM_HEAL_MONITOR_X,
                                                  GY_CHARM_HEAL_MONITOR_Y,
                                                  0);
        if (task->tBallSprite >= MAX_SPRITES || task->tMonitorSprite >= MAX_SPRITES)
        {
            if (task->tBallSprite < MAX_SPRITES)
                FieldEffectFreeGraphicsResources(&gSprites[task->tBallSprite]);
            if (task->tMonitorSprite < MAX_SPRITES)
                FieldEffectFreeGraphicsResources(&gSprites[task->tMonitorSprite]);
            sGyCharmanderHealTaskId = TASK_NONE;
            DestroyTask(taskId);
            return;
        }
        gSprites[task->tMonitorSprite].invisible = TRUE;
        PlaySE(SE_BALL);
        task->tTimer = 32;
        task->tState++;
        break;
    case 1:
        if (--task->tTimer == 0)
        {
            gSprites[task->tMonitorSprite].invisible = FALSE;
            StartSpriteAnim(&gSprites[task->tBallSprite], 1);
            StartSpriteAnim(&gSprites[task->tMonitorSprite], 1);
            PlayFanfare(MUS_HEAL);
            task->tState++;
        }
        break;
    case 2:
        if (gSprites[task->tBallSprite].animEnded
         && gSprites[task->tMonitorSprite].animEnded
         && IsFanfareTaskInactive())
        {
            FieldEffectFreeGraphicsResources(&gSprites[task->tBallSprite]);
            FieldEffectFreeGraphicsResources(&gSprites[task->tMonitorSprite]);
            sGyCharmanderHealTaskId = TASK_NONE;
            DestroyTask(taskId);
        }
        break;
    }
}

void GoldenYellowCharmander_EnsurePartnerVisible(void)
{
    struct ObjectEvent *follower;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    UpdateFollowingPokemon();
    follower = GetFollowerObject();
    if (follower != NULL && follower->active)
    {
        follower->invisible = FALSE;
        gSprites[follower->spriteId].invisible = FALSE;
        ObjectEventClearHeldMovementIfActive(follower);
    }
}

void GoldenYellowCharmander_StartSingleHeal(void)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    if (sGyCharmanderHealTaskId == TASK_NONE)
        sGyCharmanderHealTaskId = CreateTask(Task_GoldenYellowCharmanderSingleHeal, 0xFF);
}

bool8 GoldenYellowCharmander_WaitSingleHeal(void)
{
    return sGyCharmanderHealTaskId == TASK_NONE;
}

#undef tState
#undef tTimer
#undef tBallSprite
#undef tMonitorSprite
