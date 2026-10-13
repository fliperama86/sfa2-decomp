/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80017560_slot12[])(void);
extern void (*data_80017568_slot12[])(void);

void call_step_handler_then_draw_text_slot12(void) {
    data_80017560_slot12[screen_state->step_index]();
    queue_player_status_text_items();
}

void call_substep_handler_then_update_blocks_slot12(void) {
    data_80017568_slot12[screen_state->substep_index]();
    update_blocks();
    update_active_slab172_entries();
}
