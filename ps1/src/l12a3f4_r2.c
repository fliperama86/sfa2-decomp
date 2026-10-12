/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void choose_idle_state_transition_or_update(Object *object) {
    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
    } else if (is_other_object_state_0_and_1_six_one(object)) {
        set_win_state_and_store_result_byte(object);
    } else if ((u8)start_health_comparison_sequence(object)) {
        update_sequence(object);
    } else if (object->mode_flag_2 != 0) {
        update_walk_or_restart_walk_sequence_with_speed(object);
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        enter_state_1_3_with_input_variant_and_sequence(object);
    } else if (find_nearby_enemy_and_get_flag(object)) {
        enter_state_2_6_by_button_input(object);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        set_state_1_0_1_0_sequence_7(object);
    } else if ((u8)is_no_horizontal_input(object)) {
        reset_idle_or_kind_10_sequence(object);
    } else if ((u8)is_facing_differs_from_saved(object)) {
        set_win_state_or_state_4_sequence(object);
    } else {
        apply_vel_x_and_check_input_bit(object);
        update_sequence(object);
    }
}
