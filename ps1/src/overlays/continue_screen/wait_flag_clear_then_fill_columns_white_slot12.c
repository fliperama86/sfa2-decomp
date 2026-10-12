/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern int data_8002d57c_slot12;
void play_sounds_at_countdown_timer_values_slot12(void);
int spawn_object_once_with_arg_slot12(u8 a);

void wait_flag_clear_then_fill_columns_white_slot12(void) {
    while (data_8018db10) {
        update_blocks();
        update_active_slab172_entries();
        play_sounds_at_countdown_timer_values_slot12();
        request_sound_command_and_switch_thread(1);
    }
    fill_row_columns_white_once();
    data_8018f5a0->substep_index++;
}

void spawn_object_arg_3_and_run_five_frames_slot12(void) {
    data_8002d57c_slot12 = 1;
    if (spawn_object_once_with_arg_slot12(3)) {
        int n = 4;
        int t;
        do {
            update_blocks();
            update_active_slab172_entries();
            play_sounds_at_countdown_timer_values_slot12();
            request_sound_command_and_switch_thread(1);
            t = n;
            n = n - 1;
        } while (t != 0);
        data_8018f5a0->substep_index++;
        game_state.busy_flag = 0xff;
    }
}
