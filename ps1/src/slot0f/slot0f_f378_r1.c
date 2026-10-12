/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_slot0f_state_handler_a_slot0f(GameState *state);
void call_state_handler_b_then_check_pad_slot0f(GameState *state);
void reset_hud_indices_and_state_counters_slot0f(GameState *state, int a);

void init_state_and_run_main_loop_slot0f_slot0f(void) {
    GameState *s = &game_state;
    game_state.field_01 = 1;
    game_state.field_dc = 0x1b0e;
    data_8018f5a0->state_index = 0;
    data_8018f5a0->phase_index = 0;
    data_8018f5a0->step_index = 0;
    data_8018f5a0->substep_index = 0;
    data_8018f5a0->step_index_2 = 0;
    data_8018f5a0->pass_index = 0;
    game_state.step_counter = 0;
    game_state.step_delay_timer = 0;
    game_state.countdown_2 = 0;
    game_state.timer_or_bits = 0;
    game_state.countdown_timer_2 = 0;
    game_state.field_cc = 0;
    game_state.kind_index = 0;
    game_state.side_bits_b = 0;
    game_state.side_bits = 0;
    game_state.side_mask = 0;
    game_state.field_19 = 0;
    game_state.fade_busy = 0;
    game_state.wait_flag = 0;
    game_state.field_f1 = 0;
    game_state.field_09 = 0;
    game_state.state_2c_flag = 0;
    game_state.field_158 = 0;
    game_state.alt_flag = 0;
    game_state.cam_x = 0;
    game_state.cam_y = 0;
    game_state.field_d2 = 0;
    game_state.field_d4 = 0;
    game_state.cam_param_a = 0;
    game_state.cam_param_b = 0;
    game_state.table_index_2 = 7;
    func_80138358(s);
    request_sound_command_and_switch_thread(1);
    data_801a89f0 = 0;
    data_801ac61c = 0;
    {
        u32 *p = (u32 *)0x1f80000c;
        if (*p != 0) {
            *p = 0;
            reset_hud_indices_and_state_counters_slot0f(s, 0);
        }
    }
    while (1) {
        if (scratch_word_10 == 0) {
            call_slot0f_state_handler_a_slot0f(s);
        } else {
            call_state_handler_b_then_check_pad_slot0f(s);
        }
        request_sound_command_and_switch_thread(1);
    }
}
