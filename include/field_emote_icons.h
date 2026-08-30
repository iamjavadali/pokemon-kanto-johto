#ifndef GUARD_FIELD_EMOTE_ICONS_H
#define GUARD_FIELD_EMOTE_ICONS_H

struct ObjectEvent;

enum FieldEmoteIcon
{
    FIELD_EMOTE_HAPPY,
    FIELD_EMOTE_SKULL,
    FIELD_EMOTE_SLEEP,
    FIELD_EMOTE_BOLT,
    FIELD_EMOTE_FISH,
    // Animated ! from graphics/misc/emoticons.png. It is kept in the shared
    // field-emote renderer so authored scenes and non-Partner objects can
    // reuse the same presentation.
    FIELD_EMOTE_ANIMATED_EXCLAMATION,
    FIELD_EMOTE_COUNT,
};

bool32 FieldEmote_StartOnObjectEvent(const struct ObjectEvent *objectEvent, enum FieldEmoteIcon icon);
u8 FldEff_SharedEmoteIcon(void);

#endif // GUARD_FIELD_EMOTE_ICONS_H
