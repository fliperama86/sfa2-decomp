/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_state_with_input_branches_b(Object *object) {
    if (object->mode_flag_2 != 0) {
        if ((s16)object->step_flags < 0) {
            reset_object_to_state_1_0_sequence_0(object);
            return;
        }
    } else {
        if (get_pressed_low_byte(object) != 0) {
            set_state_1_0_5_and_read_input_direction(object);
            return;
        }
        if ((u8)is_input_bit_1000_held(object) != 0) {
            func_80130280(object);
            return;
        }
        if (func_8012f970(object) != 0) {
            object->state_3 = 0;
            object->field_157 = 0;
            object->facing_side = object->saved_facing;
            set_sequence_by_side(object, 0x16);
            return;
        }
        if ((u8)is_only_horizontal_input_held(object) != 0) {
            func_8012f838(object);
            return;
        }
        if ((s16)object->step_flags < 0) {
            reset_object_to_state_1_0_sequence_0(object);
            return;
        }
    }
    object->facing_side = object->saved_facing;
    update_sequence(object);
}
