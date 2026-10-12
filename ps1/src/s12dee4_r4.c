/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012e4c4(Object *object) {
    object->field_207 = 0;
    object->field_208 = 0;
    object->field_209 = 0;
    if (object->field_20c != 0) {
        func_8014cbf8(object);
    }
    if (func_80149d48(object)) {
        set_state_and_sequence_by_other_state(object);
    } else if (object->field_157 == 0) {
        reset_object_to_state_1_0_sequence_0(object);
    } else {
        reset_object_to_state_1_0_0_2_sequence_1(object);
    }
}

void call_scratch_by_side_then_bound_pos(Object *object) {
    if (object->side == 0) {
        if (object->mode_flag_2 == 0) {
            scratch_call_8c(object);
        } else {
            scratch_call_e4(object);
        }
    } else {
        if (object->mode_flag_2 == 0) {
            scratch_call_13c(object);
        } else {
            scratch_call_194(object);
        }
    }
    if (object->status_flag == 0) {
        clamp_pos_x_to_bounds_and_push_other(object);
    }
}
