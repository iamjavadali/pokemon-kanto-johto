#ifndef GUARD_FISHING_H
#define GUARD_FISHING_H

// fishing.c owns StartFishing directly and intentionally does not include this
// header. Route external fishing starts through Golden Yellow so P6 can arm the
// source one-shot modifier without changing the native fishing state machine.
void StartFishing(u8 rod);
void GoldenYellow_StartFishing(u8 rod);
#define StartFishing GoldenYellow_StartFishing

void UpdateChainFishingStreak();
u32 CalculateChainFishingShinyRolls(void);
bool32 ShouldUseFishingEnvironmentInBattle();

#endif // GUARD_FISHING_H