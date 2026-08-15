#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "main.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "rtc.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_watch.h"
#include "window.h"
#include "constants/rgb.h"

enum
{
    TRAINER_WATCH_FIELD_YEAR,
    TRAINER_WATCH_FIELD_MONTH,
    TRAINER_WATCH_FIELD_DAY,
    TRAINER_WATCH_FIELD_HOUR,
    TRAINER_WATCH_FIELD_MINUTE,
    TRAINER_WATCH_FIELD_CONFIRM,
    TRAINER_WATCH_FIELD_COUNT,
};

enum
{
    TRAINER_WATCH_STATE_EDIT,
    TRAINER_WATCH_STATE_FADE_OUT,
    TRAINER_WATCH_STATE_EXIT,
};

#define tState      data[0]
#define tYear       data[1]
#define tMonth      data[2]
#define tDay        data[3]
#define tHour       data[4]
#define tMinute     data[5]
#define tField      data[6]

static void CB2_InitTrainerWatchManualSetup(void);
static void CB2_InitTrainerWatch(void);
static void CB2_TrainerWatchManualSetup(void);
static void CB2_TrainerWatch(void);
static void VBlankCB_TrainerWatch(void);
static void Task_TrainerWatchManualSetup(u8 taskId);
static void Task_TrainerWatch(u8 taskId);
static void DrawTrainerWatchManualSetup(u8 taskId);
static void DrawTrainerWatch(void);

static const struct BgTemplate sTrainerWatchBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sTrainerWatchWindows[] =
{
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 2,
        .width = 24,
        .height = 15,
        .paletteNum = STD_WINDOW_PALETTE_NUM,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sText_Title[] = _("TRAINER WATCH");
static const u8 sText_SetDateTime[] = _("SET DATE AND TIME");
static const u8 sText_Edit[] = _("EDIT: ");
static const u8 sText_Year[] = _("YEAR");
static const u8 sText_Month[] = _("MONTH");
static const u8 sText_Day[] = _("DAY");
static const u8 sText_Hour[] = _("HOUR");
static const u8 sText_Minute[] = _("MINUTE");
static const u8 sText_Confirm[] = _("CONFIRM");
static const u8 sText_FieldHelp[] = _("LEFT/RIGHT: FIELD");
static const u8 sText_ChangeHelp[] = _("UP/DOWN: CHANGE");
static const u8 sText_ConfirmHelp[] = _("A: NEXT   B: BACK");
static const u8 sText_Back[] = _("B: BACK");
static const u8 sText_Space[] = _(" ");
static const u8 sText_CommaSpace[] = _(", ");
static const u8 sText_Colon[] = _(":");
static const u8 sText_Am[] = _(" AM");
static const u8 sText_Pm[] = _(" PM");

static const u8 sTrainerWatchTextColors[] =
{
    TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY
};

static const u8 sText_Sunday[] = _("SUNDAY");
static const u8 sText_Monday[] = _("MONDAY");
static const u8 sText_Tuesday[] = _("TUESDAY");
static const u8 sText_Wednesday[] = _("WEDNESDAY");
static const u8 sText_Thursday[] = _("THURSDAY");
static const u8 sText_Friday[] = _("FRIDAY");
static const u8 sText_Saturday[] = _("SATURDAY");

static const u8 sText_Jan[] = _("JAN");
static const u8 sText_Feb[] = _("FEB");
static const u8 sText_Mar[] = _("MAR");
static const u8 sText_Apr[] = _("APR");
static const u8 sText_May[] = _("MAY");
static const u8 sText_Jun[] = _("JUN");
static const u8 sText_Jul[] = _("JUL");
static const u8 sText_Aug[] = _("AUG");
static const u8 sText_Sep[] = _("SEP");
static const u8 sText_Oct[] = _("OCT");
static const u8 sText_Nov[] = _("NOV");
static const u8 sText_Dec[] = _("DEC");

static const u8 *const sWeekdayNames[] =
{
    sText_Sunday, sText_Monday, sText_Tuesday, sText_Wednesday,
    sText_Thursday, sText_Friday, sText_Saturday,
};

static const u8 *const sMonthNames[] =
{
    sText_Jan, sText_Feb, sText_Mar, sText_Apr, sText_May, sText_Jun,
    sText_Jul, sText_Aug, sText_Sep, sText_Oct, sText_Nov, sText_Dec,
};

static const u8 *const sFieldNames[] =
{
    [TRAINER_WATCH_FIELD_YEAR] = sText_Year,
    [TRAINER_WATCH_FIELD_MONTH] = sText_Month,
    [TRAINER_WATCH_FIELD_DAY] = sText_Day,
    [TRAINER_WATCH_FIELD_HOUR] = sText_Hour,
    [TRAINER_WATCH_FIELD_MINUTE] = sText_Minute,
    [TRAINER_WATCH_FIELD_CONFIRM] = sText_Confirm,
};

static u8 GetDaysInMonth(u16 year, u8 month)
{
    return sNumDaysInMonths[month - 1] + (month == MONTH_FEB && IsLeapYear(year));
}

static void ClampDay(struct Task *task)
{
    u8 maxDay = GetDaysInMonth(task->tYear, task->tMonth);
    if (task->tDay > maxDay)
        task->tDay = maxDay;
}

static void AdjustCurrentField(struct Task *task, s32 delta)
{
    switch (task->tField)
    {
    case TRAINER_WATCH_FIELD_YEAR:
        task->tYear += delta;
        if (task->tYear < 2000)
  task->tYear = 2099;
        else if (task->tYear > 2099)
  task->tYear = 2000;
        ClampDay(task);
        break;
    case TRAINER_WATCH_FIELD_MONTH:
        task->tMonth += delta;
        if (task->tMonth < MONTH_JAN)
  task->tMonth = MONTH_DEC;
        else if (task->tMonth > MONTH_DEC)
  task->tMonth = MONTH_JAN;
        ClampDay(task);
        break;
    case TRAINER_WATCH_FIELD_DAY:
        task->tDay += delta;
        if (task->tDay < 1)
  task->tDay = GetDaysInMonth(task->tYear, task->tMonth);
        else if (task->tDay > GetDaysInMonth(task->tYear, task->tMonth))
  task->tDay = 1;
        break;
    case TRAINER_WATCH_FIELD_HOUR:
        task->tHour += delta;
        if (task->tHour < 0)
  task->tHour = 23;
        else if (task->tHour > 23)
  task->tHour = 0;
        break;
    case TRAINER_WATCH_FIELD_MINUTE:
        task->tMinute += delta;
        if (task->tMinute < 0)
  task->tMinute = 59;
        else if (task->tMinute > 59)
  task->tMinute = 0;
        break;
    }
}

static u8 *AppendNumber(u8 *dest, s32 value, u32 digits)
{
    return ConvertIntToDecimalStringN(dest, value, STR_CONV_MODE_LEADING_ZEROS, digits);
}

static u8 GetCenteredX(const u8 *str, u8 fontId)
{
    s32 x = (24 * 8 - GetStringWidth(fontId, str, 0)) / 2;
    return x > 0 ? x : 0;
}

static void PrintTrainerWatchText(u8 fontId, const u8 *str, u8 x, u8 y)
{
    AddTextPrinterParameterized3(0, fontId, x, y, sTrainerWatchTextColors, TEXT_SKIP_DRAW, str);
}

static void FormatTrainerWatchDate(u8 *dest, u16 year, u8 month, u8 day)
{
    if (month < MONTH_JAN || month > MONTH_DEC)
        month = MONTH_JAN;

    dest = StringCopy(dest, sMonthNames[month - 1]);
    dest = StringCopy(dest, sText_Space);
    dest = ConvertIntToDecimalStringN(dest, day, STR_CONV_MODE_LEFT_ALIGN, 2);
    dest = StringCopy(dest, sText_CommaSpace);
    ConvertIntToDecimalStringN(dest, year, STR_CONV_MODE_LEADING_ZEROS, 4);
}

static void FormatTrainerWatchTime(u8 *dest, u8 hour, u8 minute)
{
    bool8 isPm = hour >= 12;
    u8 displayHour = hour % 12;

    if (displayHour == 0)
        displayHour = 12;

    dest = ConvertIntToDecimalStringN(dest, displayHour, STR_CONV_MODE_LEADING_ZEROS, 2);
    dest = StringCopy(dest, sText_Colon);
    dest = ConvertIntToDecimalStringN(dest, minute, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopy(dest, isPm ? sText_Pm : sText_Am);
}

static void DrawTrainerWatchManualSetup(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 *dest;

    FillWindowPixelBuffer(0, PIXEL_FILL(15));
    PrintTrainerWatchText(FONT_NORMAL, sText_Title, GetCenteredX(sText_Title, FONT_NORMAL), 4);
    PrintTrainerWatchText(FONT_SMALL, sText_SetDateTime, GetCenteredX(sText_SetDateTime, FONT_SMALL), 24);

    FormatTrainerWatchDate(gStringVar4, task->tYear, task->tMonth, task->tDay);
    PrintTrainerWatchText(FONT_NORMAL, gStringVar4, GetCenteredX(gStringVar4, FONT_NORMAL), 44);

    FormatTrainerWatchTime(gStringVar4, task->tHour, task->tMinute);
    PrintTrainerWatchText(FONT_NORMAL, gStringVar4, GetCenteredX(gStringVar4, FONT_NORMAL), 64);

    dest = StringCopy(gStringVar4, sText_Edit);
    StringCopy(dest, sFieldNames[task->tField]);
    PrintTrainerWatchText(FONT_SMALL, gStringVar4, GetCenteredX(gStringVar4, FONT_SMALL), 84);
    PrintTrainerWatchText(FONT_SMALL, sText_ChangeHelp, 8, 98);
    PrintTrainerWatchText(FONT_SMALL, sText_ConfirmHelp, 8, 108);

    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
    ScheduleBgCopyTilemapToVram(0);
}

static void DrawTrainerWatch(void)
{
    struct SiiRtcInfo rtc;
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 weekday;
    u8 backX;

    RtcGetInfo(&rtc);
    year = 2000 + ConvertBcdToBinary(rtc.year);
    month = ConvertBcdToBinary(rtc.month);
    day = ConvertBcdToBinary(rtc.day);
    hour = ConvertBcdToBinary(rtc.hour);
    minute = ConvertBcdToBinary(rtc.minute);
    weekday = rtc.dayOfWeek < WEEKDAY_COUNT ? rtc.dayOfWeek : WEEKDAY_SUN;

    FillWindowPixelBuffer(0, PIXEL_FILL(15));
    PrintTrainerWatchText(FONT_NORMAL, sText_Title, GetCenteredX(sText_Title, FONT_NORMAL), 4);
    PrintTrainerWatchText(FONT_SMALL, sWeekdayNames[weekday], GetCenteredX(sWeekdayNames[weekday], FONT_SMALL), 32);

    FormatTrainerWatchDate(gStringVar4, year, month, day);
    PrintTrainerWatchText(FONT_NORMAL, gStringVar4, GetCenteredX(gStringVar4, FONT_NORMAL), 50);

    FormatTrainerWatchTime(gStringVar4, hour, minute);
    PrintTrainerWatchText(FONT_NORMAL, gStringVar4, GetCenteredX(gStringVar4, FONT_NORMAL), 72);

    backX = (24 * 8) - GetStringWidth(FONT_SMALL, sText_Back, 0) - 8;
    PrintTrainerWatchText(FONT_SMALL, sText_Back, backX, 102);
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
    ScheduleBgCopyTilemapToVram(0);
}

static void Task_TrainerWatch(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case TRAINER_WATCH_STATE_EDIT:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 1, 0, 16, RGB_WHITEALPHA);
            task->tState = TRAINER_WATCH_STATE_FADE_OUT;
        }
        break;
    case TRAINER_WATCH_STATE_FADE_OUT:
        if (!gPaletteFade.active)
            task->tState = TRAINER_WATCH_STATE_EXIT;
        break;
    case TRAINER_WATCH_STATE_EXIT:
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetVBlankCallback(NULL);
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        break;
    }
}

static void CB2_InitTrainerWatch(void)
{
    u8 taskId;

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetVBlankCallback(NULL);
    DmaClear16(3, PLTT, PLTT_SIZE);
    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
    ResetOamRange(0, 128);
    LoadOam();
    ScanlineEffect_Stop();
    ScanlineEffect_Clear();
    ResetSpriteData();
    ResetTasks();
    ResetPaletteFade();
    ClearScheduledBgCopiesToVram();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sTrainerWatchBgTemplates, ARRAY_COUNT(sTrainerWatchBgTemplates));
    InitWindows(sTrainerWatchWindows);
    DeactivateAllTextPrinters();
    Menu_LoadStdPal();
    LoadUserWindowBorderGfx(0, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
    SetStandardWindowBorderStyle(0, FALSE);
    ShowBg(0);

    taskId = CreateTask(Task_TrainerWatch, 80);
    gTasks[taskId].tState = TRAINER_WATCH_STATE_EDIT;
    DrawTrainerWatch();

    BeginNormalPaletteFade(PALETTES_ALL, 1, 16, 0, RGB_WHITEALPHA);
    SetVBlankCallback(VBlankCB_TrainerWatch);
    SetMainCallback2(CB2_TrainerWatch);
}

static void CB2_TrainerWatch(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void Task_TrainerWatchManualSetup(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
    case TRAINER_WATCH_STATE_EDIT:
        if (JOY_NEW(DPAD_LEFT))
        {
  task->tField = (task->tField == 0) ? TRAINER_WATCH_FIELD_CONFIRM : task->tField - 1;
  DrawTrainerWatchManualSetup(taskId);
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
  task->tField = (task->tField + 1) % TRAINER_WATCH_FIELD_COUNT;
  DrawTrainerWatchManualSetup(taskId);
        }
        else if (JOY_REPEAT(DPAD_UP))
        {
  AdjustCurrentField(task, 1);
  DrawTrainerWatchManualSetup(taskId);
        }
        else if (JOY_REPEAT(DPAD_DOWN))
        {
  AdjustCurrentField(task, -1);
  DrawTrainerWatchManualSetup(taskId);
        }
        else if (JOY_NEW(B_BUTTON))
        {
  if (task->tField > 0)
      task->tField--;
  DrawTrainerWatchManualSetup(taskId);
        }
        else if (JOY_NEW(A_BUTTON))
        {
  if (task->tField != TRAINER_WATCH_FIELD_CONFIRM)
  {
      task->tField++;
      DrawTrainerWatchManualSetup(taskId);
  }
  else
  {
      TrainerWatchSetManualDateTime(task->tYear, task->tMonth, task->tDay, task->tHour, task->tMinute);
      BeginNormalPaletteFade(PALETTES_ALL, 1, 0, 16, RGB_WHITEALPHA);
      task->tState = TRAINER_WATCH_STATE_FADE_OUT;
  }
        }
        break;
    case TRAINER_WATCH_STATE_FADE_OUT:
        if (!gPaletteFade.active)
  task->tState = TRAINER_WATCH_STATE_EXIT;
        break;
    case TRAINER_WATCH_STATE_EXIT:
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetVBlankCallback(NULL);
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        break;
    }
}

static void CB2_InitTrainerWatchManualSetup(void)
{
    u8 taskId;

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetVBlankCallback(NULL);
    DmaClear16(3, PLTT, PLTT_SIZE);
    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
    ResetOamRange(0, 128);
    LoadOam();
    ScanlineEffect_Stop();
    ScanlineEffect_Clear();
    ResetSpriteData();
    ResetTasks();
    ResetPaletteFade();
    ClearScheduledBgCopiesToVram();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sTrainerWatchBgTemplates, ARRAY_COUNT(sTrainerWatchBgTemplates));
    InitWindows(sTrainerWatchWindows);
    DeactivateAllTextPrinters();
    Menu_LoadStdPal();
    LoadUserWindowBorderGfx(0, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
    SetStandardWindowBorderStyle(0, FALSE);
    ShowBg(0);

    taskId = CreateTask(Task_TrainerWatchManualSetup, 80);
    gTasks[taskId].tState = TRAINER_WATCH_STATE_EDIT;
    gTasks[taskId].tYear = 2026;
    gTasks[taskId].tMonth = MONTH_JAN;
    gTasks[taskId].tDay = 1;
    gTasks[taskId].tHour = 12;
    gTasks[taskId].tMinute = 0;
    gTasks[taskId].tField = TRAINER_WATCH_FIELD_YEAR;
    DrawTrainerWatchManualSetup(taskId);

    BeginNormalPaletteFade(PALETTES_ALL, 1, 16, 0, RGB_WHITEALPHA);
    SetVBlankCallback(VBlankCB_TrainerWatch);
    SetMainCallback2(CB2_TrainerWatchManualSetup);
}

static void CB2_TrainerWatchManualSetup(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_TrainerWatch(void)
{
    ProcessSpriteCopyRequests();
    LoadOam();
    TransferPlttBuffer();
}

void StartTrainerWatchManualSetup(void)
{
    SetMainCallback2(CB2_InitTrainerWatchManualSetup);
}

void StartTrainerWatch(void)
{
    SetMainCallback2(CB2_InitTrainerWatch);
}
