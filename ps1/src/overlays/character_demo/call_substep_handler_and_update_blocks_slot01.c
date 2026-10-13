/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8001514c_slot01[])(void);

void call_substep_handler_and_update_blocks_slot01(void) {
    data_8001514c_slot01[screen_state->substep_index]();
    update_blocks();
    call_active_block_handlers();
    select_buffers_and_run_first_setup();
}
