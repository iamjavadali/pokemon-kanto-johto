// Compatibility include retained so the existing C translation unit does not need
// unrelated edits. scaninc recognizes #include inside .inc files and follows these
// headers. event_data.h exposes the canonical script special variables used by the
// portrait entry points; the asset header exposes literal INCGFX dependencies.
#include "event_data.h"
#include "golden_yellow_pikapic_assets.h"