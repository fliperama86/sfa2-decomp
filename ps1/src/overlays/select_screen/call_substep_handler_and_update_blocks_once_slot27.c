/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80017bc4_slot27[])(void);
extern void (*data_80017bdc_slot27[])(void);
extern u8 data_80029324_slot27;

void call_substep_handler_and_update_blocks_once_slot27(void) {
    int one;
    data_80017bc4_slot27[data_8018f5a0->substep_index]();
    one = 1;
    data_80190568 = one;
    update_blocks();
    call_active_block_handlers();
    if (data_80029324_slot27 != 0 && game_state.setup_done_flag == 0) {
        update_active_slab172_entries();
        call_block_update_for_enabled_entries();
        game_state.setup_done_flag = one;
    }
    queue_player_status_text_items();
}

void call_handler_by_hud_step_index_2_slot27(void) {
    data_80017bdc_slot27[data_8018f5a0->step_index_2]();
}
