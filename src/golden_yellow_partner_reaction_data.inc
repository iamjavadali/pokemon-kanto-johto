/*
 * Pokemon Yellow source:
 *   data/pikachu/pikachu_emotions.asm
 *
 * Preserve source command order exactly. Game Boy-only setup/restore
 * subcommands are represented as semantic callbacks and intentionally remain
 * inert during P3 debug execution.
 */
#define GY_REACTION_CMD(type_, arg_, value_) \
    { .type = (type_), .arg = (arg_), .value = (value_) }
#define GY_REACTION_END() \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_END, 0, 0)
#define GY_REACTION_CRY(cry_) \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_CRY, (cry_), 0)
#define GY_REACTION_BUBBLE(bubble_) \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_BUBBLE, (bubble_), 0)
#define GY_REACTION_MOVEMENT(movement_) \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_MOVEMENT, (movement_), 0)
#define GY_REACTION_PORTRAIT(program_) \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_PORTRAIT, (program_), 0)
#define GY_REACTION_DELAY(frames_) \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_DELAY, 0, (frames_))
#define GY_REACTION_TURN_AWAY() \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_TURN_AWAY, 0, 0)
#define GY_REACTION_CALLBACK(callback_) \
    GY_REACTION_CMD(GY_PARTNER_REACTION_CMD_CALLBACK, (callback_), 0)

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction0[] =
{
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction1[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_DEFAULT),
    GY_REACTION_PORTRAIT(1),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction2[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_SMILE),
    GY_REACTION_CRY(GY_PARTNER_CRY_35),
    GY_REACTION_PORTRAIT(2),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction3[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_40),
    GY_REACTION_PORTRAIT(3),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction4[] =
{
    GY_REACTION_MOVEMENT(GY_PARTNER_MOVEMENT_PLAYFUL_HOP),
    GY_REACTION_CRY(GY_PARTNER_CRY_29),
    GY_REACTION_PORTRAIT(4),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction5[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_31),
    GY_REACTION_PORTRAIT(5),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction6[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_DEFAULT),
    GY_REACTION_MOVEMENT(GY_PARTNER_MOVEMENT_ALOOF_TURN),
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_SKULL),
    GY_REACTION_PORTRAIT(6),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction7[] =
{
    GY_REACTION_MOVEMENT(GY_PARTNER_MOVEMENT_ENERGETIC_HOP),
    GY_REACTION_CRY(GY_PARTNER_CRY_1),
    GY_REACTION_MOVEMENT(GY_PARTNER_MOVEMENT_ENERGETIC_HOP),
    GY_REACTION_PORTRAIT(7),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction8[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_39),
    GY_REACTION_PORTRAIT(8),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction9[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_6),
    GY_REACTION_MOVEMENT(GY_PARTNER_MOVEMENT_AGITATED_TURN),
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_SKULL),
    GY_REACTION_PORTRAIT(9),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction10[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_HEART),
    GY_REACTION_CRY(GY_PARTNER_CRY_5),
    GY_REACTION_PORTRAIT(10),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction11[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_ZZZ),
    GY_REACTION_CRY(GY_PARTNER_CRY_37),
    GY_REACTION_PORTRAIT(11),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction12[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_DEFAULT),
    GY_REACTION_PORTRAIT(12),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction13[] =
{
    GY_REACTION_MOVEMENT(GY_PARTNER_MOVEMENT_ALOOF_TURN),
    GY_REACTION_PORTRAIT(13),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction14[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_BOLT),
    GY_REACTION_CRY(GY_PARTNER_CRY_10),
    GY_REACTION_PORTRAIT(14),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction15[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_34),
    GY_REACTION_PORTRAIT(15),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction16[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_33),
    GY_REACTION_PORTRAIT(16),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction17[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_13),
    GY_REACTION_PORTRAIT(17),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction18[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_DEFAULT),
    GY_REACTION_PORTRAIT(18),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction19[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_HEART),
    GY_REACTION_CRY(GY_PARTNER_CRY_33),
    GY_REACTION_PORTRAIT(19),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction20[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_HEART),
    GY_REACTION_CRY(GY_PARTNER_CRY_5),
    GY_REACTION_PORTRAIT(20),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction21[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_FISH),
    GY_REACTION_CRY(GY_PARTNER_CRY_DEFAULT),
    GY_REACTION_PORTRAIT(21),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction22[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_4),
    GY_REACTION_PORTRAIT(22),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction23[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_19),
    GY_REACTION_PORTRAIT(23),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_SHOW_MAP_VIEW),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction24[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_EXCLAMATION),
    GY_REACTION_CRY(GY_PARTNER_CRY_DEFAULT),
    GY_REACTION_PORTRAIT(24),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction25[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_BOLT),
    GY_REACTION_CRY(GY_PARTNER_CRY_35),
    GY_REACTION_PORTRAIT(25),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction26[] =
{
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_ZZZ),
    GY_REACTION_CRY(GY_PARTNER_CRY_37),
    GY_REACTION_PORTRAIT(26),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_SHOW_MAP_VIEW),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_PEWTER_CENTER),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction27[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_9),
    GY_REACTION_PORTRAIT(27),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction28[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_15),
    GY_REACTION_PORTRAIT(28),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction29[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_5),
    GY_REACTION_PORTRAIT(10),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction30[] =
{
    GY_REACTION_TURN_AWAY(),
    GY_REACTION_BUBBLE(GY_PARTNER_BUBBLE_HEART),
    GY_REACTION_CRY(GY_PARTNER_CRY_5),
    GY_REACTION_PORTRAIT(20),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_SHOW_MAP_VIEW),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_LOAD_FONT),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_FAN_CLUB),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction31[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_19),
    GY_REACTION_PORTRAIT(23),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_SHOW_MAP_VIEW),
    GY_REACTION_CALLBACK(GY_PARTNER_CALLBACK_BILL_HOUSE),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand sPartnerReaction32[] =
{
    GY_REACTION_CRY(GY_PARTNER_CRY_26),
    GY_REACTION_PORTRAIT(23),
    GY_REACTION_END(),
};

static const struct GoldenYellowPartnerReactionCommand *const sPartnerReactionPrograms[GY_PARTNER_REACTION_COUNT] =
{
    [GY_PARTNER_REACTION_EMPTY]                  = sPartnerReaction0,
    [GY_PARTNER_REACTION_NEUTRAL]                = sPartnerReaction1,
    [GY_PARTNER_REACTION_HAPPY]                  = sPartnerReaction2,
    [GY_PARTNER_REACTION_SUBDUED]                = sPartnerReaction3,
    [GY_PARTNER_REACTION_PLAYFUL]                = sPartnerReaction4,
    [GY_PARTNER_REACTION_DISPLEASED]             = sPartnerReaction5,
    [GY_PARTNER_REACTION_IRRITATED]              = sPartnerReaction6,
    [GY_PARTNER_REACTION_ENERGETIC_HAPPY]        = sPartnerReaction7,
    [GY_PARTNER_REACTION_HAPPY_GRIN]             = sPartnerReaction8,
    [GY_PARTNER_REACTION_STRONG_DISPLEASURE]     = sPartnerReaction9,
    [GY_PARTNER_REACTION_AFFECTION]              = sPartnerReaction10,
    [GY_PARTNER_REACTION_SLEEPING]               = sPartnerReaction11,
    [GY_PARTNER_REACTION_FROWNING]               = sPartnerReaction12,
    [GY_PARTNER_REACTION_ALOOF]                  = sPartnerReaction13,
    [GY_PARTNER_REACTION_VERY_ANGRY]             = sPartnerReaction14,
    [GY_PARTNER_REACTION_CONTENT]                = sPartnerReaction15,
    [GY_PARTNER_REACTION_STRONG_HAPPINESS]       = sPartnerReaction16,
    [GY_PARTNER_REACTION_UNHAPPY]                = sPartnerReaction17,
    [GY_PARTNER_REACTION_CAPTURE_SUCCESS]        = sPartnerReaction18,
    [GY_PARTNER_REACTION_STRONG_AFFECTION]       = sPartnerReaction19,
    [GY_PARTNER_REACTION_MAX_AFFECTION]          = sPartnerReaction20,
    [GY_PARTNER_REACTION_FISHING]                = sPartnerReaction21,
    [GY_PARTNER_REACTION_TOWER_AFRAID]           = sPartnerReaction22,
    [GY_PARTNER_REACTION_BILL_CONFUSED]          = sPartnerReaction23,
    [GY_PARTNER_REACTION_THUNDER_STONE_REFUSAL]  = sPartnerReaction24,
    [GY_PARTNER_REACTION_ELECTRIC_POWER]         = sPartnerReaction25,
    [GY_PARTNER_REACTION_PEWTER_JIGGLYPUFF]      = sPartnerReaction26,
    [GY_PARTNER_REACTION_BILL_SHOCKED]           = sPartnerReaction27,
    [GY_PARTNER_REACTION_STATUS_SICK]            = sPartnerReaction28,
    [GY_PARTNER_REACTION_FAN_CLUB_AFFECTION]     = sPartnerReaction29,
    [GY_PARTNER_REACTION_FAN_CLUB_MAX_AFFECTION] = sPartnerReaction30,
    [GY_PARTNER_REACTION_BILL_POST_STATE]        = sPartnerReaction31,
    [GY_PARTNER_REACTION_BILL_INTERMEDIATE]      = sPartnerReaction32,
};

#undef GY_REACTION_CMD
#undef GY_REACTION_END
#undef GY_REACTION_CRY
#undef GY_REACTION_BUBBLE
#undef GY_REACTION_MOVEMENT
#undef GY_REACTION_PORTRAIT
#undef GY_REACTION_DELAY
#undef GY_REACTION_TURN_AWAY
#undef GY_REACTION_CALLBACK
