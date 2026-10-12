/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_state_with_input_branches_a(Object *object) {
    u16 saved;
    if (object->mode_flag_2 != 0) {
        update_sequence_or_return_to_idle(object);
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        enter_state_1_3_with_input_variant_and_sequence(object);
    } else if (find_nearby_enemy_and_get_flag(object)) {
        enter_state_2_6_by_button_input(object);
    } else if ((u8)is_only_horizontal_input_held(object)) {
        start_move_state_1_0_2_1_with_sequence_by_input(object);
    } else if (!(u8)is_button_4000_held_or_other_flag(object)) {
        saved = object->step_timer;
        object->state_3 = 0;
        set_sequence_by_side(object, (u8)object->step_flags + 10);
        object->step_timer = saved;
    } else if ((s16)object->step_flags < 0) {
        reset_object_to_state_1_0_0_2_sequence_1(object);
    } else {
        object->facing_side = object->saved_facing;
        update_sequence(object);
    }
}

void tick_timers_and_call_scratch_by_side(Object *object) {
    if (object->field_7e != 0) {
        move_object_x_by_table_value(object);
    }
    count_down_countdown_4_2(object);
    if (object->mode_flag_2 == 0) {
        if (object->kind_timer != 0) {
            object->kind_timer--;
        }
        if (object->countdown_5 != 0) {
            object->countdown_5--;
        }
        if (object->kind_table_value != 0) {
            object->kind_table_value--;
        }
    }
    if (object->side == 0) {
        if (object->mode_flag_2 != 0) {
            scratch_call_d8(object);
        } else {
            scratch_call_54(object);
        }
    } else {
        if (object->mode_flag_2 != 0) {
            scratch_call_188(object);
        } else {
            scratch_call_104(object);
        }
    }
}
