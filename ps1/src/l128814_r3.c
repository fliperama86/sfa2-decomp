/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void choose_transition_or_update_sequence(Object *object) {
    object->field_157 = 0;
    if (object->field_7e != 0) {
        move_object_x_by_table_value(object);
        if (object->field_7e == 1) {
            object->field_225 = 0;
            set_sequence_by_side(object, 0);
        }
    }
    if (game_state.config->mode_flag_b != 0) {
        set_win_or_lose_state_by_health(object);
    } else if (is_other_object_state_0_and_1_six_one(object)) {
        set_win_state_and_store_result_byte(object);
    } else if ((u8)start_health_comparison_sequence(object) == 0 && game_state.config->mode_flag_c == 0) {
        if ((u8)is_facing_differs_from_saved(object)) {
            set_win_state_or_state_4_sequence(object);
        } else if (object->mode_flag_2 == 0) {
            if (get_pressed_low_byte(object)) {
                set_state_1_0_5_and_read_input_direction(object);
            } else if ((u8)is_button_4000_held_or_other_flag(object)) {
                set_state_1_0_1_0_sequence_7(object);
            } else if (object->field_7e == 0) {
                if ((u8)is_input_bit_1000_held(object)) {
                    enter_state_1_3_with_input_variant_and_sequence(object);
                } else if (find_nearby_enemy_and_get_flag(object)) {
                    enter_state_2_6_by_button_input(object);
                } else if ((u8)is_single_horizontal_input_held(object)) {
                    start_move_state_1_0_2_1_with_sequence_by_input(object);
                } else {
                    update_sequence(object);
                }
            } else {
                update_sequence(object);
            }
        } else {
            update_sequence(object);
        }
    } else {
        update_sequence(object);
    }
}

void select_transition_by_conditions_1(Object *object) {
    object->field_157 = 1;
    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
    } else if (is_other_object_state_0_and_1_six_one(object)) {
        set_win_state_and_store_result_byte(object);
    } else if ((u8)start_health_comparison_sequence(object) != 0) {
        update_sequence_or_set_step_flags_kind_6(object);
    } else if ((u8)is_facing_differs_from_saved(object)) {
        set_win_state_or_state_4_sequence(object);
    } else if (object->mode_flag_2 != 0) {
        update_sequence_or_set_step_flags_kind_6(object);
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        if ((u8)is_input_bit_1000_held(object)) {
            enter_state_1_3_with_input_variant_and_sequence(object);
        } else if (find_nearby_enemy_and_get_flag(object)) {
            enter_state_2_6_by_button_input(object);
        } else {
            update_sequence_or_set_step_flags_kind_6(object);
        }
    } else if (object->field_7e == 0 && (u8)is_single_horizontal_input_held(object)) {
        start_move_state_1_0_2_1_with_sequence_by_input(object);
    } else {
        set_state_1_0_1_2_sequence_4(object);
    }
}
