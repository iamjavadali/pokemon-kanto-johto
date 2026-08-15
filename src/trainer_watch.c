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
static void CB2_TrainerWatchManualSetup(void);
static void VBlankCB_TrainerWatch(void);
static void Task_TrainerWatchManualSetup(u8 taskId);
static void DrawTrainerWatchManualSetup(u8 taskId);

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
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 28,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sText_Title[] = _("TRAINER WATCH");
static const u8 sText_NoRtc[] = _("Automatic RTC wasn't found.\nSet the current date and time.");
static const u8 sText_Date[] = _("DATE  ");
static const u8 sText_Time[] = _("TIME  ");
static const u8 sText_Edit[] = _("EDIT  ");
static const u8 sText_Year[] = _("YEAR");
static const u8 sText_Month[] = _("MONTH");
static const u8 sText_Day[] = _("DAY");
static const u8 sText_Hour[] = _("HOUR");
static const u8 sText_Minute[] = _("MINUTE");
static const u8 sText_Confirm[] = _("CONFIRM");
static const u8 sText_Help[] = _("UP/DOWN: CHANGE\nLEFT/RIGHT: FIELD\nA: NEXT / CONFIRM\nB: PREVIOUS FIELD");
static const u8 sText_Hyphen[] = _("-");
static const u8 sText_Colon[] = _(":");

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

static void DrawTrainerWatchManualSetup(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 *dest;

    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    AddTextPrinterParameterized(0, FONT_NORMAL, sText_Title, 8, 4, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(0, FONT_SMALL, sText_NoRtc, 8, 24, TEXT_SKIP_DRAW, NULL);

    dest = StringCopy(gStringVar4, sText_Date);
    dest = AppendNumber(dest, task->tYear, 4);
    dest = StringCopy(dest, sText_Hyphen);
    dest = AppendNumber(dest, task->tMonth, 2);
    dest = StringCopy(dest, sText_Hyphen);
    AppendNumber(dest, task->tDay, 2);
    AddTextPrinterParameterized(0, FONT_NORMAL, gStringVar4, 8, 56, TEXT_SKIP_DRAW, NULL);

    dest = StringCopy(gStringVar4, sText_Time);
    dest = AppendNumber(dest, task->tHour, 2);
    dest = StringCopy(dest, sText_Colon);
    AppendNumber(dest, task->tMinute, 2);
    AddTextPrinterParameterized(0, FONT_NORMAL, gStringVar4, 8, 76, TEXT_SKIP_DRAW, NULL);

    dest = StringCopy(gStringVar4, sText_Edit);
    StringCopy(dest, sFieldNames[task->tField]);
    AddTextPrinterParameterized(0, FONT_NORMAL, gStringVar4, 8, 100, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(0, FONT_SMALL, sText_Help, 8, 120, TEXT_SKIP_DRAW, NULL);

    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
    ScheduleBgCopyTilemapToVram(0);
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
    LoadMessageBoxAndBorderGfx();
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
