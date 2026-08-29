#ifndef GUARD_GOLDEN_YELLOW_PARTNER_REACTION_H
#define GUARD_GOLDEN_YELLOW_PARTNER_REACTION_H

struct ScriptContext;

// Direct Yellow PikaPic source programs are 0-28. Program 29 is rejected by
// Yellow's original loader and aliases back to program 0, so callers and the
// debug browser must never treat 29 as a separately selectable reaction.
void GoldenYellow_ShowPartnerPikachuPortrait(struct ScriptContext *ctx);
void GoldenYellow_ShowPartnerPikachuPortraitProgram(struct ScriptContext *ctx);
bool32 GoldenYellow_DebugStartPikachuPortraitBrowser(void);

#endif // GUARD_GOLDEN_YELLOW_PARTNER_REACTION_H
