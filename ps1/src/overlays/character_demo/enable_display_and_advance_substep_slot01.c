/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

int try_spawn_pooled_object_once_with_arg_slot01(u8 a);

void enable_display_and_advance_substep_slot01(void) {
    func_80157d00(1);
    if (game_state.busy_flag == 0) {
        data_8018f5a0->substep_index++;
        game_state.field_09 = 0;
        game_state.state_2c_flag = 0;
        game_state.timer_or_bits = 0x12c;
    }
}

void wait_input_or_timer_then_next_substep_slot01(void) {
    int t;
    if (game_state.field_04 == 0) {
        if (is_any_enabled_pad_button_pressed() != 0 || (t = game_state.timer_or_bits - 1, game_state.timer_or_bits = t, (s16)t < 0)) {
            set_menu_command_by_code(6, 2);
            data_8018f5a0->substep_index++;
            game_state.status_flags = 0xff;
            game_state.field_09 = 1;
            game_state.state_2c_flag = 0xff;
        }
    }
}

void wait_fade_then_start_job_two_and_advance_slot01(void) {
    while (game_state.wait_flag != 0) {
        request_sound_command_and_switch_thread(1);
    }
    start_fade_mode_1_and_wait();
    if (try_spawn_pooled_object_once_with_arg_slot01(2) != 0) {
        game_state.busy_flag = 0xff;
        data_8018f5a0->substep_index++;
    }
}
