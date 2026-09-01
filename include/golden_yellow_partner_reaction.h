#ifndef GUARD_GOLDEN_YELLOW_PARTNER_REACTION_H
#define GUARD_GOLDEN_YELLOW_PARTNER_REACTION_H

struct ScriptContext;

// Direct Yellow PikaPic source programs are 0-28. Program 29 is rejected by
// Yellow's original loader and aliases back to program 0, so callers and the
// debug browser must never treat 29 as a separately selectable portrait.
void GoldenYellow_ShowPartnerPikachuPortrait(struct ScriptContext *ctx);
void GoldenYellow_ShowPartnerPikachuPortraitProgram(struct ScriptContext *ctx);
bool32 GoldenYellow_DebugStartPikachuPortraitBrowser(void);

// P3 reaction director. Reaction IDs preserve Yellow Emotion 0-32; normal
// A-button selection and story precedence are intentionally integrated later.
bool32 GoldenYellow_StartPartnerPikachuReaction(u8 reactionId);
bool32 GoldenYellow_StartPartnerPikachuFieldTalkReaction(u8 reactionId);
bool32 GoldenYellow_IsPartnerPikachuReactionActive(void);
bool32 GoldenYellow_DebugStartPikachuReactionBrowser(void);

// Authored maps may temporarily render canonical Partner Pikachu as a normal
// scene-owned object while the generic follower is deliberately suppressed.
// These helpers change only which visible object the existing P3 director
// targets; they do not replace its reaction programs, movement, emotes, or
// portrait renderer.
void GoldenYellow_SetPartnerPikachuReactionObject(u8 objectEventId);
void GoldenYellow_ClearPartnerPikachuReactionObject(void);

// P4 field-interaction adapter. EventScript_Follower calls this before the
// expansion's generic follower cry/action path. VAR_RESULT is TRUE only when
// the interaction belongs to canonical Partner Pikachu.
void GoldenYellow_TryPartnerPikachuFieldInteraction(struct ScriptContext *ctx);

// Internal bridge used by the P3 director to reuse the accepted P2 renderer
// without attaching a second native wait to the active field script.
bool32 GoldenYellow_StartPartnerPikachuPortraitForReaction(u8 programId);
bool32 GoldenYellow_IsPartnerPikachuPortraitActive(void);

#endif // GUARD_GOLDEN_YELLOW_PARTNER_REACTION_H
