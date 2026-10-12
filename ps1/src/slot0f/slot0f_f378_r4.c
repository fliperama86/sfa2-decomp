/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800e83c8_slot0f[])(GameState *state);
void check_buttons_and_call_reset_slot0f(GameState *state);

void advance_state_index_reset_phase_slot0f(GameState *state) {
    state->wait_flag = 0;
    data_8018f5a0->phase_index = 0;
    data_8018f5a0->state_index++;
}

void func_800df810_slot0f(GameState *state) {
    request_sound_command_and_switch_thread(10);
    scratch_word_00 = -1;
    scratch_word_04 = 1;
    scratch_word_10 = 1;
    load_and_exec_program_by_index(1);
}

void call_state_handler_b_then_check_pad_slot0f(GameState *state) {
    data_800e83c8_slot0f[data_8018f5a0->state_index](state);
    check_buttons_and_call_reset_slot0f(state);
}
