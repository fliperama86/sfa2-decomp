/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

u8 call_script_subop_from_next_word(void) {
    u16 *p = script_cursor;
    script_cursor = p + 1;
    data_8017d3d4[(s16)*p >> 1]();
    return 1;
}
