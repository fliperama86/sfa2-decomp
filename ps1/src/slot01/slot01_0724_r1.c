/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8001336c_slot01(u8 arg);

void spawn_object_and_loop_while_busy_slot01(void) {
    if (func_8001336c_slot01(1)) {
        game_state.busy_flag = 0xff;
    }
    while (game_state.busy_flag != 0) {
        update_blocks();
        call_active_block_handlers();
        select_buffers_and_run_first_setup();
        request_sound_command_and_switch_thread(1);
    }
}

void func_80010790_slot01(void) {
    if (func_8001336c_slot01(0)) {
        HudState *h = data_8018f5a0;
        game_state.busy_flag = 0xff;
        h->substep_index++;
    }
}

void increment_substep_index_slot01(void) {
    data_8018f5a0->substep_index++;
}
