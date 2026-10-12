/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void set_sequence_x28_if_state_7_0_1_and_active(Object *object) {
    if ((*(u32 *)&object->state_0 & 0xffffff) == 0x70001 &&
        object->field_15a == 1 && object->field_7e == 0 &&
        object->active_flag != 0) {
        object->active_flag = 0;
        set_sequence_by_side_and_mode(object, 0x28);
    }
}

int is_input_and_state_check_passed(Object *object) {
    int result;
    if (object->mode_flag_2 != 0) {
        result = 0;
    } else if (!(u8)is_input_masked_equal_to_one_of_two_patterns_b(object)) {
        result = 0;
    } else {
        result = (u8)try_action_unless_state_bytes_match(object) != 0;
    }
    return result;
}

void set_state_7_variant_from_input(Object *object) {
    object->variant_index = 1;
    if (!(object->input_bits_2 & 0x8000)) object->variant_index = 0xff;
    object->field_15a = 2;
    object->active_flag_3 = 0;
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 7;
    object->state_3 = 0;
    reset_substate_and_set_direction_index(object);
}

void reset_substate_and_set_direction_index(Object *object) {
    u8 side = object->saved_facing;
    u16 bits = object->pressed_bits | object->released_bits;
    u8 value;
    object->substate_index = 0;
    object->field_12d = 0;
    object->step_count_2 = 0;
    object->step_limit = 0;
    object->facing_side = side;
    if (bits & 0xc0) {
        value = 0;
    } else if (bits & 0x30) {
        value = 2;
    } else {
        value = 4;
    }
    object->direction_index = value;
}
