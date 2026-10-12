/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


int is_pad_pressed_for_leader_bits_or_mode(void) {
    u8 result = 0;
    if (game_state.mode != 3) {
        return is_any_enabled_pad_button_pressed();
    }
    if (game_state.leader_bits & 1) {
        if (is_pad_button_pressed()) {
            result = 1;
        }
    }
    if (game_state.leader_bits & 2) {
        if (is_pad_button_pressed_2()) {
            result |= 1;
        }
    }
    return result;
}

u8 is_pad_button_pressed(void) {
    return (data_801a696a & 0x9f0) != 0;
}

u8 is_pad_button_pressed_2(void) {
    return (data_801a6976 & 0x9f0) != 0;
}

void call_input_step_handler(void) {
    if (game_state.focus_object->mode_flag_2 == 0 && game_state.field_0c == 0 && (game_state.input_enable_bits & 1)) {
        table_8016e99c[game_state.input_step_index]();
    }
}
