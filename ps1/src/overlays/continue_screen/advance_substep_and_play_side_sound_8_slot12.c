/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002d568_slot12;

void advance_substep_and_play_side_sound_8_slot12(void) {
    screen_state->substep_index = screen_state->substep_index + 1;
    set_menu_command_by_code(6, 2);
    game_state.timer_or_bits = 0x3c;
    game_state.status_flags = 0xff;
    play_side_sound_at(data_8002d568_slot12, data_8002d568_slot12->side, 8);
}

void reset_hud_and_game_state_to_phase_4_slot12(void) {
    set_menu_command_by_code(6, 2);
    screen_state->phase_index = 4;
    game_state.step_delay_timer = 0;
    screen_state->step_index = 0;
    game_state.countdown_2 = 0;
    screen_state->substep_index = 0;
    game_state.timer_or_bits = 0;
    screen_state->step_index_2 = 0;
    game_state.countdown_timer_2 = 0;
    screen_state->pass_index = 0;
    game_state.field_cc = 0;
    game_state.active_flag = 0;
    game_state.state_2c_flag = 0;
    game_state.bit_mask = 0;
    game_state.field_65 = 1;
    game_state.status_flags = 1;
    game_state.field_4f = 0;
    game_state.field_09 = 1;
    game_state.state_46_flag = 0;
    game_state.field_44 = 1;
}
