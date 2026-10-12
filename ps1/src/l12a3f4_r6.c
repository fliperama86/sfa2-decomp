/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012ba80(Object *object) {
    if (object->mode_flag_2 != 0) {
        if ((s16)object->step_flags < 0) {
            func_80131468(object);
        } else {
            object->facing_side = object->saved_facing;
            update_sequence(object);
        }
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_80130258(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        object->state_3 = 2;
        object->field_157 = 1;
        object->facing_side = object->saved_facing;
        set_sequence_by_side(object, 0x19);
    } else if ((u8)func_8012f800(object)) {
        func_8012f838(object);
    } else if ((s16)object->step_flags < 0) {
        func_80131468(object);
    } else {
        object->facing_side = object->saved_facing;
        update_sequence(object);
    }
}
