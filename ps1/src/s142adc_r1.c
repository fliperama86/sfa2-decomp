/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void start_action_if_buttons_pressed(Object *object) {
    if (object->field_49 != 0 && object->active_flag_4 == 0 && get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    }
}

void set_direction_index_from_buttons_group_a(Object *object) {
    reset_hold_flags_and_other_hit_count(object);
    if (((object->pressed_bits | object->released_bits) & 0x80) || ((object->pressed_bits | object->released_bits) & 1)) {
        object->direction_index = 0;
    } else if ((object->pressed_bits | object->released_bits) & 0x10) {
        object->direction_index = 2;
    } else {
        object->direction_index = 4;
    }
}

void set_direction_index_from_buttons_group_b(Object *object) {
    reset_hold_flags_and_other_hit_count(object);
    if (((object->pressed_bits | object->released_bits) & 0x40) || ((object->pressed_bits | object->released_bits) & 2)) {
        object->direction_index = 0;
    } else if ((object->pressed_bits | object->released_bits) & 0x20) {
        object->direction_index = 2;
    } else {
        object->direction_index = 4;
    }
}

void reset_hold_flags_and_other_hit_count(Object *object) {
    object->field_49 = 0;
    object->field_29c = 0;
    object->sequence_hold_flag = 0;
    if (object->field_7e != 0) object->field_49 = object->field_49 + 1;
    ref_other.p = object->other;
    if (object->sound_side != 0) ref_other.p->hit_count_b = 0;
    else ref_other.p->hit_count_a = 0;
}
