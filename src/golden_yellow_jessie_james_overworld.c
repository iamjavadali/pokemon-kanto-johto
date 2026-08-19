#include "global.h"
#include "event_object_movement.h"
#include "sprite.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"

#define GOLDEN_YELLOW_ROCKET_FRAME_SIZE 256
#define GOLDEN_YELLOW_ROCKET_FRAME_WORDS (GOLDEN_YELLOW_ROCKET_FRAME_SIZE / sizeof(u32))

static const u32 sObjectEventPic_Jessie[] =
    INCGFX_U32("graphics/object_events/pics/people/jessie.png", ".4bpp", "-mwidth 2 -mheight 4");
static const u32 sObjectEventPic_James[] =
    INCGFX_U32("graphics/object_events/pics/people/james.png", ".4bpp", "-mwidth 2 -mheight 4");

#define ROCKET_FRAME(gfx, frame) \
    { .data = (gfx) + GOLDEN_YELLOW_ROCKET_FRAME_WORDS * (frame), .size = GOLDEN_YELLOW_ROCKET_FRAME_SIZE }

static const struct SpriteFrameImage sPicTable_Jessie[] =
{
    ROCKET_FRAME(sObjectEventPic_Jessie, 0),
    ROCKET_FRAME(sObjectEventPic_Jessie, 1),
    ROCKET_FRAME(sObjectEventPic_Jessie, 2),
    ROCKET_FRAME(sObjectEventPic_Jessie, 3),
    ROCKET_FRAME(sObjectEventPic_Jessie, 4),
    ROCKET_FRAME(sObjectEventPic_Jessie, 5),
    ROCKET_FRAME(sObjectEventPic_Jessie, 6),
    ROCKET_FRAME(sObjectEventPic_Jessie, 7),
    ROCKET_FRAME(sObjectEventPic_Jessie, 8),
};

static const struct SpriteFrameImage sPicTable_James[] =
{
    ROCKET_FRAME(sObjectEventPic_James, 0),
    ROCKET_FRAME(sObjectEventPic_James, 1),
    ROCKET_FRAME(sObjectEventPic_James, 2),
    ROCKET_FRAME(sObjectEventPic_James, 3),
    ROCKET_FRAME(sObjectEventPic_James, 4),
    ROCKET_FRAME(sObjectEventPic_James, 5),
    ROCKET_FRAME(sObjectEventPic_James, 6),
    ROCKET_FRAME(sObjectEventPic_James, 7),
    ROCKET_FRAME(sObjectEventPic_James, 8),
};

static const union AnimCmd sAnim_JessieJamesFaceSouth[] = { ANIMCMD_FRAME(0, 8), ANIMCMD_JUMP(0) };
static const union AnimCmd sAnim_JessieJamesFaceNorth[] = { ANIMCMD_FRAME(1, 8), ANIMCMD_JUMP(0) };
static const union AnimCmd sAnim_JessieJamesFaceWest[]  = { ANIMCMD_FRAME(2, 8), ANIMCMD_JUMP(0) };
static const union AnimCmd sAnim_JessieJamesFaceEast[]  = { ANIMCMD_FRAME(2, 8, .hFlip = TRUE), ANIMCMD_JUMP(0) };

#define DEFINE_ROCKET_WALK_ANIM(name, a, b, c, d, delay, flip) \
static const union AnimCmd name[] = \
{ \
    ANIMCMD_FRAME(a, delay, .hFlip = flip), \
    ANIMCMD_FRAME(b, delay, .hFlip = flip), \
    ANIMCMD_FRAME(c, delay, .hFlip = flip), \
    ANIMCMD_FRAME(d, delay, .hFlip = flip), \
    ANIMCMD_JUMP(0), \
}

DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoSouth,       3, 0, 4, 0, 8, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoNorth,       5, 1, 6, 1, 8, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoWest,        7, 2, 8, 2, 8, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoEast,        7, 2, 8, 2, 8, TRUE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastSouth,   3, 0, 4, 0, 4, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastNorth,   5, 1, 6, 1, 4, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastWest,    7, 2, 8, 2, 4, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastEast,    7, 2, 8, 2, 4, TRUE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFasterSouth, 3, 0, 4, 0, 2, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFasterNorth, 5, 1, 6, 1, 2, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFasterWest,  7, 2, 8, 2, 2, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFasterEast,  7, 2, 8, 2, 2, TRUE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastestSouth,3, 0, 4, 0, 1, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastestNorth,5, 1, 6, 1, 1, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastestWest, 7, 2, 8, 2, 1, FALSE);
DEFINE_ROCKET_WALK_ANIM(sAnim_JessieJamesGoFastestEast, 7, 2, 8, 2, 1, TRUE);

static const union AnimCmd *const sAnimTable_JessieJames[] =
{
    [ANIM_STD_FACE_SOUTH] = sAnim_JessieJamesFaceSouth,
    [ANIM_STD_FACE_NORTH] = sAnim_JessieJamesFaceNorth,
    [ANIM_STD_FACE_WEST] = sAnim_JessieJamesFaceWest,
    [ANIM_STD_FACE_EAST] = sAnim_JessieJamesFaceEast,
    [ANIM_STD_GO_SOUTH] = sAnim_JessieJamesGoSouth,
    [ANIM_STD_GO_NORTH] = sAnim_JessieJamesGoNorth,
    [ANIM_STD_GO_WEST] = sAnim_JessieJamesGoWest,
    [ANIM_STD_GO_EAST] = sAnim_JessieJamesGoEast,
    [ANIM_STD_GO_FAST_SOUTH] = sAnim_JessieJamesGoFastSouth,
    [ANIM_STD_GO_FAST_NORTH] = sAnim_JessieJamesGoFastNorth,
    [ANIM_STD_GO_FAST_WEST] = sAnim_JessieJamesGoFastWest,
    [ANIM_STD_GO_FAST_EAST] = sAnim_JessieJamesGoFastEast,
    [ANIM_STD_GO_FASTER_SOUTH] = sAnim_JessieJamesGoFasterSouth,
    [ANIM_STD_GO_FASTER_NORTH] = sAnim_JessieJamesGoFasterNorth,
    [ANIM_STD_GO_FASTER_WEST] = sAnim_JessieJamesGoFasterWest,
    [ANIM_STD_GO_FASTER_EAST] = sAnim_JessieJamesGoFasterEast,
    [ANIM_STD_GO_FASTEST_SOUTH] = sAnim_JessieJamesGoFastestSouth,
    [ANIM_STD_GO_FASTEST_NORTH] = sAnim_JessieJamesGoFastestNorth,
    [ANIM_STD_GO_FASTEST_WEST] = sAnim_JessieJamesGoFastestWest,
    [ANIM_STD_GO_FASTEST_EAST] = sAnim_JessieJamesGoFastestEast,
};

const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_Jessie =
{
    .tileTag = TAG_NONE,
    .paletteTag = OBJ_EVENT_PAL_TAG_NPC_PINK,
    .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,
    .size = GOLDEN_YELLOW_ROCKET_FRAME_SIZE,
    .width = 16,
    .height = 32,
    .paletteSlot = PALSLOT_NPC_2,
    .shadowSize = SHADOW_SIZE_M,
    .inanimate = FALSE,
    .compressed = FALSE,
    .tracks = TRACKS_FOOT,
    .oam = &gObjectEventBaseOam_16x32,
    .subspriteTables = sOamTables_16x32,
    .anims = sAnimTable_JessieJames,
    .images = sPicTable_Jessie,
};

const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_James =
{
    .tileTag = TAG_NONE,
    .paletteTag = OBJ_EVENT_PAL_TAG_NPC_PINK,
    .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,
    .size = GOLDEN_YELLOW_ROCKET_FRAME_SIZE,
    .width = 16,
    .height = 32,
    .paletteSlot = PALSLOT_NPC_2,
    .shadowSize = SHADOW_SIZE_M,
    .inanimate = FALSE,
    .compressed = FALSE,
    .tracks = TRACKS_FOOT,
    .oam = &gObjectEventBaseOam_16x32,
    .subspriteTables = sOamTables_16x32,
    .anims = sAnimTable_JessieJames,
    .images = sPicTable_James,
};

#undef DEFINE_ROCKET_WALK_ANIM
#undef ROCKET_FRAME
#undef GOLDEN_YELLOW_ROCKET_FRAME_WORDS
#undef GOLDEN_YELLOW_ROCKET_FRAME_SIZE
