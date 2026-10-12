/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_spawn_pooled_object_once_with_arg_slot01(u8 arg);

void spawn_object_and_loop_while_busy_slot01(void) {
    if (try_spawn_pooled_object_once_with_arg_slot01(1)) {
        game_state.busy_flag = 0xff;
    }
    while (game_state.busy_flag != 0) {
        update_blocks();
        call_active_block_handlers();
        select_buffers_and_run_first_setup();
        request_sound_command_and_switch_thread(1);
    }
}

void start_job_zero_and_advance_substep_slot01(void) {
    if (try_spawn_pooled_object_once_with_arg_slot01(0)) {
        HudState *h = data_8018f5a0;
        game_state.busy_flag = 0xff;
        h->substep_index++;
    }
}

void increment_substep_index_slot01(void) {
    data_8018f5a0->substep_index++;
}
