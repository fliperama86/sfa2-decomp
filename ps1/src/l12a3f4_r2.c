/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012a684(Object *object) {
    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
    } else if (func_8012f56c(object)) {
        set_win_state_and_store_result_byte(object);
    } else if ((u8)start_health_comparison_sequence(object)) {
        update_sequence(object);
    } else if (object->mode_flag_2 != 0) {
        func_8012a804(object);
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        enter_state_2_6_by_button_input(object);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        set_state_1_0_1_0_sequence_7(object);
    } else if ((u8)is_no_horizontal_input(object)) {
        reset_idle_or_kind_10_sequence(object);
    } else if ((u8)is_facing_differs_from_saved(object)) {
        func_8012f8c4(object);
    } else {
        apply_vel_x_and_check_input_bit(object);
        update_sequence(object);
    }
}
