#include "global.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "field_emote_icons.h"
#include "palette.h"
#include "sprite.h"
#include "constants/event_objects.h"
#include "constants/field_effects.h"

#define SHARED_EMOTE_FRAMES 60

static const u8 sFieldEmoteHappyGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_happy.png", ".4bpp");
static const u8 sFieldEmoteSkullGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_skull.png", ".4bpp");
static const u8 sFieldEmoteSleepGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_sleep.png", ".4bpp");
static const u8 sFieldEmoteBoltGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_bolt.png", ".4bpp");
static const u8 sFieldEmoteFishGfx[] =
    INCGFX_U8("graphics/field_effects/pics/emotion_fish.png", ".4bpp");

static const struct OamData sFieldEmoteOam =
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

static const struct SpriteFrameImage sFieldEmoteImages[] =
{
    { .data = sFieldEmoteHappyGfx, .size = sizeof(sFieldEmoteHappyGfx) },
    { .data = sFieldEmoteSkullGfx, .size = sizeof(sFieldEmoteSkullGfx) },
    { .data = sFieldEmoteSleepGfx, .size = sizeof(sFieldEmoteSleepGfx) },
    { .data = sFieldEmoteBoltGfx, .size = sizeof(sFieldEmoteBoltGfx) },
    { .data = sFieldEmoteFishGfx, .size = sizeof(sFieldEmoteFishGfx) },
};

static const union AnimCmd sFieldEmoteAnimHappy[] =
{
    ANIMCMD_FRAME(FIELD_EMOTE_HAPPY, SHARED_EMOTE_FRAMES),
    ANIMCMD_END,
};

static const union AnimCmd sFieldEmoteAnimSkull[] =
{
    ANIMCMD_FRAME(FIELD_EMOTE_SKULL, SHARED_EMOTE_FRAMES),
    ANIMCMD_END,
};

static const union AnimCmd sFieldEmoteAnimSleep[] =
{
    ANIMCMD_FRAME(FIELD_EMOTE_SLEEP, SHARED_EMOTE_FRAMES),
    ANIMCMD_END,
};

static const union AnimCmd sFieldEmoteAnimBolt[] =
{
    ANIMCMD_FRAME(FIELD_EMOTE_BOLT, SHARED_EMOTE_FRAMES),
    ANIMCMD_END,
};

static const union AnimCmd sFieldEmoteAnimFish[] =
{
    ANIMCMD_FRAME(FIELD_EMOTE_FISH, SHARED_EMOTE_FRAMES),
    ANIMCMD_END,
};

static const union AnimCmd *const sFieldEmoteAnims[] =
{
    [FIELD_EMOTE_HAPPY] = sFieldEmoteAnimHappy,
    [FIELD_EMOTE_SKULL] = sFieldEmoteAnimSkull,
    [FIELD_EMOTE_SLEEP] = sFieldEmoteAnimSleep,
    [FIELD_EMOTE_BOLT] = sFieldEmoteAnimBolt,
    [FIELD_EMOTE_FISH] = sFieldEmoteAnimFish,
};

static void SpriteCB_SharedEmoteIcon(struct Sprite *sprite);

static const struct SpriteTemplate sFieldEmoteTemplate =
{
    .tileTag = TAG_NONE,
    // The uploaded icons deliberately use the same 0/14/15 palette indexes
    // as FRLG's existing ?, !, and heart field icons.
    .paletteTag = OBJ_EVENT_PAL_TAG_MAY,
    .oam = &sFieldEmoteOam,
    .anims = sFieldEmoteAnims,
    .images = sFieldEmoteImages,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SharedEmoteIcon,
};

#define sLocalId    data[0]
#define sMapNum     data[1]
#define sMapGroup   data[2]
#define sYVelocity  data[3]
#define sYOffset    data[4]
#define sFldEffId   data[7]

static void SetSharedEmoteSpriteData(struct Sprite *sprite, enum FieldEmoteIcon icon)
{
    sprite->oam.priority = 1;
    sprite->coordOffsetEnabled = TRUE;

    sprite->sLocalId = gFieldEffectArguments[0];
    sprite->sMapNum = gFieldEffectArguments[1];
    sprite->sMapGroup = gFieldEffectArguments[2];
    sprite->sYVelocity = -5;
    sprite->sFldEffId = FLDEFF_SHARED_EMOTE_ICON;

    StartSpriteAnim(sprite, icon);
}

u8 FldEff_SharedEmoteIcon(void)
{
    enum FieldEmoteIcon icon = gFieldEffectArguments[7];
    u8 spriteId;

    if (icon >= FIELD_EMOTE_COUNT)
    {
        FieldEffectActiveListRemove(FLDEFF_SHARED_EMOTE_ICON);
        return 0;
    }

    spriteId = CreateSpriteAtEnd(&sFieldEmoteTemplate, 0, 0, 0x52);
    if (spriteId == MAX_SPRITES)
    {
        FieldEffectActiveListRemove(FLDEFF_SHARED_EMOTE_ICON);
        return 0;
    }

    SetSharedEmoteSpriteData(&gSprites[spriteId], icon);
    UpdateSpritePaletteByTemplate(&sFieldEmoteTemplate, &gSprites[spriteId]);
    return 0;
}

bool32 FieldEmote_StartOnObjectEvent(const struct ObjectEvent *objectEvent, enum FieldEmoteIcon icon)
{
    if (objectEvent == NULL
     || !objectEvent->active
     || icon >= FIELD_EMOTE_COUNT
     || FieldEffectActiveListContains(FLDEFF_SHARED_EMOTE_ICON))
        return FALSE;

    gFieldEffectArguments[0] = objectEvent->localId;
    gFieldEffectArguments[1] = gSaveBlock1Ptr->location.mapNum;
    gFieldEffectArguments[2] = gSaveBlock1Ptr->location.mapGroup;
    gFieldEffectArguments[7] = icon;
    FieldEffectStart(FLDEFF_SHARED_EMOTE_ICON);
    return TRUE;
}

static void SpriteCB_SharedEmoteIcon(struct Sprite *sprite)
{
    u8 objectEventId;

    if (TryGetObjectEventIdByLocalIdAndMap(sprite->sLocalId, sprite->sMapNum, sprite->sMapGroup, &objectEventId)
     || sprite->animEnded)
    {
        FieldEffectStop(sprite, sprite->sFldEffId);
    }
    else
    {
        struct Sprite *objectEventSprite = &gSprites[gObjectEvents[objectEventId].spriteId];

        sprite->sYOffset += sprite->sYVelocity;
        sprite->x = objectEventSprite->x;
        sprite->y = objectEventSprite->y - 16;
        sprite->x2 = objectEventSprite->x2;
        sprite->y2 = objectEventSprite->y2 + sprite->sYOffset;
        if (sprite->sYOffset)
            sprite->sYVelocity++;
        else
            sprite->sYVelocity = 0;
    }
}

#undef sLocalId
#undef sMapNum
#undef sMapGroup
#undef sYVelocity
#undef sYOffset
#undef sFldEffId
