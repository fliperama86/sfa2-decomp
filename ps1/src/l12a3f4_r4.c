/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012b184(Object *object) {
    u16 saved;
    if (object->mode_flag_2 != 0) {
        update_sequence_or_return_to_idle(object);
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_input_bit_1000_held(object)) {
        func_80130280(object);
    } else if ((u8)is_single_horizontal_input_held(object)) {
        func_8012f838(object);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        object->state_3 = object->state_3 + 1;
        saved = object->step_timer;
        set_sequence_by_side(object, (u8)object->step_flags + 0xd);
        object->step_timer = saved;
    } else if (func_8012f970(object)) {
        enter_state_2_6_by_button_input(object);
    } else if ((s16)object->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(object);
    } else {
        object->facing_side = object->saved_facing;
        update_sequence(object);
    }
}
