/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800151b4_slot01[])(void);

void call_substep_handler_then_update_blocks_slot01(void) {
    data_800151b4_slot01[data_8018f5a0->substep_index]();
    update_blocks();
    update_active_slab172_entries();
}
