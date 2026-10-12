/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

u16 get_pressed_low_byte(Object *object) {
    return (u8)object->pressed_bits;
}


void set_state_1_0_5_and_read_input_direction(Object *object) {
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 5;
    object->state_3 = 0;
    if (object->field_7e == 0) object->facing_side = object->saved_facing;
    reset_hold_flags_and_other_hit_count(object);
    object->active_flag = 0;
    set_direction_from_pressed_bits(object);
    object->field_157 = ((u8)is_button_4000_held_or_other_flag(object) != 0) << 1;
    object->input_dir_0 = object->field_157;
    tick_timers_and_call_scratch_by_side(object);
}

void set_direction_from_pressed_bits(Object *object) {
    BytePair d = get_direction_pair_from_pressed_bits(object);
    object->direction_index = d.first;
    object->input_dir_second = d.second;
}

BytePair get_direction_pair_from_pressed_bits(Object *object) {
    BytePair d;
    u16 pad = object->pressed_bits;
    d.second = 0;
    d.first = 0;
    if (pad & 0x80) {
        d.first = 0;
        d.second = 0;
    } else if (pad & 0x10) {
        d.first = 2;
        d.second = 0;
    } else if (pad & 0x4) {
        d.first = 4;
        d.second = 0;
    } else if (pad & 0x40) {
        d.first = 0;
        d.second = 2;
    } else if (pad & 0x20) {
        d.first = 2;
        d.second = 2;
    } else if (pad & 0x8) {
        d.first = 4;
        d.second = 2;
    } else if (pad & 0x1) {
        d.first = 0;
        d.second = 0;
    } else if (pad & 0x2) {
        d.first = 0;
        d.second = 2;
    }
    return d;
}
