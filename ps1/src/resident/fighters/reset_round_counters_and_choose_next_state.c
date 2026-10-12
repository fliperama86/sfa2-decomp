/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void reset_round_counters_and_choose_next_state(Object *object) {
    add_round_score_to_other_side(object);
    object->field_69 = 0;
    object->round_count = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        u8 count;

        count = object->field_1a1 - 1;
        object->field_1a1 = count;
        if (count & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_170 = 0;
    object->field_6b = 0;
    object->field_28a = 0;
    object->hit_count_a = 0;
    object->hit_count_b = 0;
    clear_flag_if_second_flag_set(object);
    object->other_state_code = 5;
    object->facing_side = object->saved_facing;
    if (object->mode_flag_2 != 0) {
        clear_script_bytes_and_select_reset_transition(object);
    } else {
        object->hold_timer = 2;
        if (get_pressed_low_byte(object) != 0) {
            set_state_1_0_5_and_read_input_direction(object);
        } else if (find_nearby_enemy_and_get_flag(object) != 0) {
            enter_state_2_6_by_button_input(object);
        } else if (object->input_bits_2 & 0x4000) {
            reset_object_to_state_1_0_0_2_sequence_1(object);
        } else {
            reset_object_to_state_1_0_sequence_0(object);
        }
    }
}
