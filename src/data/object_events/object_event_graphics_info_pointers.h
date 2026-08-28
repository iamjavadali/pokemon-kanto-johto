// Golden Yellow reserves two legacy unused doll graphics IDs for Jessie and James.
// Keeping the original pointer table in a base include preserves every existing
// object-event numeric ID while these two unused slots gain dedicated 32x32 actors.
#define gObjectEventGraphicsInfo_UnusedNatuDoll gObjectEventGraphicsInfo_Jessie
#define gObjectEventGraphicsInfo_UnusedMagnemiteDoll gObjectEventGraphicsInfo_James

#include "object_event_graphics_info_pointers_base.h"

#undef gObjectEventGraphicsInfo_UnusedMagnemiteDoll
#undef gObjectEventGraphicsInfo_UnusedNatuDoll
