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
    FIELD_EMOTE_COUNT,
};

bool32 FieldEmote_StartOnObjectEvent(const struct ObjectEvent *objectEvent, enum FieldEmoteIcon icon);
u8 FldEff_SharedEmoteIcon(void);

#endif // GUARD_FIELD_EMOTE_ICONS_H
