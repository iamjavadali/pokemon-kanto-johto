static const struct YellowPikaPicFrame sPikaFrames6[] = {{0,2}, {14,4}, {0,8}, {14,4}, {0,64}, {14,4}, {0,64}};
static const struct YellowPikaPicFrame sPikaFrames7[] = {{0,4}, {15,4}, {0,4}, {15,4}, {0,8}, {15,4}, {0,8}, {15,4}};
static const struct YellowPikaPicFrame sPikaFrames8[] = {{16,1}, {0,1}, {16,1}, {0,64}, {16,1}, {0,64}};
static const struct YellowPikaPicFrame sPikaFrames9[] = {{0,8}, {17,8}, {0,20}, {17,8}};
static const struct YellowPikaPicFrame sPikaFrames10[] = {{0,2}, {18,2}, {0,2}, {18,64}, {0,3}, {18,64}};
static const struct YellowPikaPicFrame sPikaFrames11[] = {{0,8}, {19,64}, {0,4}, {19,64}};
static const struct YellowPikaPicFrame sPikaFrames12[] = {{20,8}, {0,2}, {20,8}, {0,2}, {20,8}};
static const struct YellowPikaPicFrame sPikaFrames13[] = {{21,4}, {0,8}, {21,4}, {0,64}, {21,4}, {0,64}};
static const struct YellowPikaPicFrame sPikaFrames14[] = {{0,2}, {22,2}, {0,2}, {22,2}, {0,20}, {22,2}};
static const struct YellowPikaPicFrame sPikaFrames16[] = {{0,8}, {23,3}, {24,5}, {23,3}, {0,5}};
static const struct YellowPikaPicFrame sPikaFrames17[] = {{0,20}, {25,8}, {0,20}, {25,8}};
static const struct YellowPikaPicFrame sPikaFrames18[] = {{0,13}, {26,12}, {0,100}, {26,8}};
static const struct YellowPikaPicFrame sPikaFrames19[] = {{0,5}, {27,5}, {0,5}, {27,5}, {0,100}};
static const struct YellowPikaPicFrame sPikaFrames20[] = {{0,2}, {28,2}, {0,2}, {28,2}};
static const struct YellowPikaPicFrame sPikaFrames21[] = {{0,5}, {29,5}, {0,5}, {29,5}};
static const struct YellowPikaPicFrame sPikaFrames22[] = {{30,8}, {0,100}};
static const struct YellowPikaPicFrame sPikaFrames23[] = {{0,10}, {31,3}, {0,3}, {31,3}, {0,100}};
static const struct YellowPikaPicFrame sPikaFrames24[] = {{0,3}, {32,100}, {0,8}, {32,8}};
static const struct YellowPikaPicFrame sPikaFrames25[] = {{33,6}, {0,6}, {33,6}, {0,6}};
static const struct YellowPikaPicFrame sPikaFrames26[] = {{0,8}, {34,12}, {0,8}, {34,12}};
static const struct YellowPikaPicFrame sPikaFrames27[] = {{0,8}, {9,2}, {10,1}, {11,1}, {12,100}};
static const struct YellowPikaPicFrame sPikaFrames28[] = {{0,8}, {36,100}};
static const struct YellowPikaPicFrame sPikaFrames29[] = {{0,16}, {37,16}, {0,16}, {37,16}};
static const struct YellowPikaPicFrame sPikaFrames30[] = {{0,6}, {38,6}, {0,6}, {38,6}, {0,100}};
static const struct YellowPikaPicFrame sPikaFrames31[] = {{0,6}, {9,6}, {10,100}};
static const struct YellowPikaPicFrame sPikaFrames32[] = {{0,20}, {9,8}, {0,20}, {9,8}, {10,8}, {11,100}};
static const struct YellowPikaPicFrame sPikaFrames33[] = {{0,4}, {9,100}};
static const struct YellowPikaPicFrame sPikaFrames34[] = {{0,12}, {9,12}, {0,12}, {9,100}};

#define PATCH1(id_, a_) static const struct YellowPikaPicPatchAsset sPikaPatches##id_[] = { PATCH_ASSET(sPikaGfx_##a_) }
#define PATCH2(id_, a_, b_) static const struct YellowPikaPicPatchAsset sPikaPatches##id_[] = { PATCH_ASSET(sPikaGfx_##a_), PATCH_ASSET(sPikaGfx_##b_) }
#define PATCH3(id_, a_, b_, c_) static const struct YellowPikaPicPatchAsset sPikaPatches##id_[] = { PATCH_ASSET(sPikaGfx_##a_), PATCH_ASSET(sPikaGfx_##b_), PATCH_ASSET(sPikaGfx_##c_) }
#define PATCH4(id_, a_, b_, c_, d_) static const struct YellowPikaPicPatchAsset sPikaPatches##id_[] = { PATCH_ASSET(sPikaGfx_##a_), PATCH_ASSET(sPikaGfx_##b_), PATCH_ASSET(sPikaGfx_##c_), PATCH_ASSET(sPikaGfx_##d_) }
PATCH1(0, e40cc);
PATCH1(1, e40cc);
PATCH1(2, e41d2);
PATCH1(3, e4323);
PATCH1(4, e444b);
PATCH1(5, e463b);
PATCH1(6, e472e);
PATCH1(7, e4841);
PATCH1(8, e4a99);
PATCH1(9, e4bde);
PATCH2(10, e4ce0, e4e70);
PATCH1(11, e50af);
PATCH1(12, e52fe);
PATCH1(13, e5541);
PATCH1(14, e5794);
PATCH1(15, e59ed);
PATCH1(16, e5c4d);
PATCH1(17, e5e90);
PATCH1(18, e61b0);
PATCH1(19, e63f7);
PATCH1(20, e6646);
PATCH4(21, e682f, e69bf, e6b4f, e6cdf);
PATCH1(22, e6fff);
PATCH1(23, e731f);
PATCH1(24, e763f);
PATCH2(25, e7863, e79f3);
PATCH3(26, e50af, e7b83, e7d13);
PATCH1(27, f0b64);
PATCH1(28, f0d82);
#undef PATCH1
#undef PATCH2
#undef PATCH3
#undef PATCH4

#define PIKA_PROGRAM(id_, base_, bg_, duration_) \
    [id_] = { \
        .baseGfx = sPikaGfx_##base_, \
        .patchAssets = sPikaPatches##id_, \
        .frames = sPikaFrames##bg_, \
        .durationTicks = duration_, \
        .patchAssetCount = ARRAY_COUNT(sPikaPatches##id_), \
        .frameCount = ARRAY_COUNT(sPikaFrames##bg_), \
    }
static const struct YellowPikaPicProgram sPikaPrograms[YELLOW_PIKAPIC_PROGRAM_COUNT] =
{
    PIKA_PROGRAM(0, e4000, 6, 40),
    PIKA_PROGRAM(1, e4000, 6, 40),
    PIKA_PROGRAM(2, e411c, 7, 44),
    PIKA_PROGRAM(3, e4272, 8, 80),
    PIKA_PROGRAM(4, e4383, 9, 70),
    PIKA_PROGRAM(5, e458b, 10, 32),
    PIKA_PROGRAM(6, e467b, 11, 50),
    PIKA_PROGRAM(7, e476e, 12, 58),
    PIKA_PROGRAM(8, e49d1, 13, 44),
    PIKA_PROGRAM(9, e4b39, 14, 56),
    PIKA_PROGRAM(10, e4c3e, 16, 56),
    PIKA_PROGRAM(11, e5000, 17, 100),
    PIKA_PROGRAM(12, e523f, 18, 50),
    PIKA_PROGRAM(13, e548e, 19, 50),
    PIKA_PROGRAM(14, e56d1, 20, 40),
    PIKA_PROGRAM(15, e5924, 21, 50),
    PIKA_PROGRAM(16, e5b7d, 22, 32),
    PIKA_PROGRAM(17, e5ddd, 23, 100),
    PIKA_PROGRAM(18, e6020, 24, 32),
    PIKA_PROGRAM(19, e6340, 25, 44),
    PIKA_PROGRAM(20, e6587, 26, 50),
    PIKA_PROGRAM(21, e67d6, 27, 40),
    PIKA_PROGRAM(22, e6e6f, 28, 40),
    PIKA_PROGRAM(23, e718f, 29, 70),
    PIKA_PROGRAM(24, e74af, 30, 60),
    PIKA_PROGRAM(25, e77cf, 31, 50),
    PIKA_PROGRAM(26, e5000, 32, 100),
    PIKA_PROGRAM(27, f0abf, 33, 30),
    PIKA_PROGRAM(28, f0cf4, 34, 64),
};
#undef PIKA_PROGRAM
