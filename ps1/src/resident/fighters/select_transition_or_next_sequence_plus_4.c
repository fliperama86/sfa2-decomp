/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void select_transition_or_next_sequence_plus_4(Object *object) {
    u16 saved;
    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
    } else if (is_other_object_state_0_and_1_six_one(object)) {
        set_win_state_and_store_result_byte(object);
    } else if (object->mode_flag_2 != 0) {
        if ((u8)is_facing_differs_from_saved(object)) {
            set_win_state_or_state_4_sequence(object);
        } else if ((s16)object->step_flags < 0) {
            reset_object_to_state_1_0_0_2_sequence_1(object);
        } else {
            update_sequence(object);
        }
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        enter_state_1_3_with_input_variant_and_sequence(object);
    } else if (find_nearby_enemy_and_get_flag(object)) {
        enter_state_2_6_by_button_input(object);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        if ((u8)is_facing_differs_from_saved(object)) {
            set_win_state_or_state_4_sequence(object);
        } else if ((s16)object->step_flags < 0) {
            reset_object_to_state_1_0_0_2_sequence_1(object);
        } else {
            update_sequence(object);
        }
    } else if ((u8)is_single_horizontal_input_held(object)) {
        start_move_state_1_0_2_1_with_sequence_by_input(object);
    } else {
        saved = object->step_timer;
        object->state_3++;
        set_sequence_by_side(object, (u8)object->step_flags + 4);
        object->step_timer = saved;
    }
}
