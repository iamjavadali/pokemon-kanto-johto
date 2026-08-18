#include "global.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "palette.h"
#include "script.h"
#include "sprite.h"

#define TAG_GOLDEN_YELLOW_SLEEP_EMOTE 0x1015
#define SLEEP_EMOTE_FRAMES 60

static const u8 sGoldenYellowSleepEmoteGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_sleep.png", ".4bpp");
static const u16 sGoldenYellowSleepEmotePal[] =
    INCGFX_U16("graphics/field_effects/pics/emotion_sleep.png", ".gbapal");

static void SpriteCB_GoldenYellowSleepEmote(struct Sprite *sprite);

static const struct OamData sGoldenYellowSleepEmoteOam =
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

static const struct SpriteFrameImage sGoldenYellowSleepEmoteImages[] =
{
    {
        .data = sGoldenYellowSleepEmoteGfx,
        .size = sizeof(sGoldenYellowSleepEmoteGfx),
    },
};

static const union AnimCmd sGoldenYellowSleepEmoteAnim[] =
{
    ANIMCMD_FRAME(0, SLEEP_EMOTE_FRAMES),
    ANIMCMD_END,
};

static const union AnimCmd *const sGoldenYellowSleepEmoteAnims[] =
{
    sGoldenYellowSleepEmoteAnim,
};

static const struct SpritePalette sGoldenYellowSleepEmotePalette =
{
    .data = sGoldenYellowSleepEmotePal,
    .tag = TAG_GOLDEN_YELLOW_SLEEP_EMOTE,
};

static const struct SpriteTemplate sGoldenYellowSleepEmoteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = TAG_GOLDEN_YELLOW_SLEEP_EMOTE,
    .oam = &sGoldenYellowSleepEmoteOam,
    .anims = sGoldenYellowSleepEmoteAnims,
    .images = sGoldenYellowSleepEmoteImages,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_GoldenYellowSleepEmote,
};

#define sFollowerObjectEventId data[0]

void GoldenYellow_ShowFollowerSleepEmote(void)
{
    struct ObjectEvent *follower = GetFollowerObject();
    u8 spriteId;

    if (follower == NULL || !follower->active)
    {
        ScriptContext_Enable();
        return;
    }

    LoadSpritePalette(&sGoldenYellowSleepEmotePalette);
    spriteId = CreateSprite(&sGoldenYellowSleepEmoteTemplate, 0, 0, 0x52);
    if (spriteId == MAX_SPRITES)
    {
        FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_SLEEP_EMOTE);
        ScriptContext_Enable();
        return;
    }

    gSprites[spriteId].coordOffsetEnabled = TRUE;
    gSprites[spriteId].sFollowerObjectEventId = follower - gObjectEvents;
    SpriteCB_GoldenYellowSleepEmote(&gSprites[spriteId]);
}

void GoldenYellow_FacePlayerTowardFollower(void)
{
    struct ObjectEvent *follower = GetFollowerObject();
    struct ObjectEvent *player;
    s16 deltaX;
    s16 deltaY;
    enum Direction direction;

    if (follower == NULL || !follower->active)
        return;

    player = &gObjectEvents[gPlayerAvatar.objectEventId];
    deltaX = follower->currentCoords.x - player->currentCoords.x;
    deltaY = follower->currentCoords.y - player->currentCoords.y;

    if (deltaX > 0)
        direction = DIR_EAST;
    else if (deltaX < 0)
        direction = DIR_WEST;
    else if (deltaY > 0)
        direction = DIR_SOUTH;
    else if (deltaY < 0)
        direction = DIR_NORTH;
    else
        return;

    PlayerTurnInPlace(direction);
}

static void SpriteCB_GoldenYellowSleepEmote(struct Sprite *sprite)
{
    u8 objectEventId = sprite->sFollowerObjectEventId;

    if (objectEventId >= OBJECT_EVENTS_COUNT
     || !gObjectEvents[objectEventId].active
     || sprite->animEnded)
    {
        DestroySprite(sprite);
        FreeSpritePaletteByTag(TAG_GOLDEN_YELLOW_SLEEP_EMOTE);
        ScriptContext_Enable();
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

#undef sFollowerObjectEventId
