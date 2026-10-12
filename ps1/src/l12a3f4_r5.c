/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012b324(Object *object) {
    u16 saved;
    if (object->mode_flag_2 != 0) {
        func_8012b2c4(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_80130258(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if ((u8)func_8012f800(object)) {
        func_8012f838(object);
    } else if (!(u8)func_8012f618(object)) {
        saved = object->step_timer;
        object->state_3 = 0;
        set_sequence_by_side(object, (u8)object->step_flags + 10);
        object->step_timer = saved;
    } else if ((s16)object->step_flags < 0) {
        func_80131468(object);
    } else {
        object->facing_side = object->saved_facing;
        update_sequence(object);
    }
}

void func_8012b45c(Object *object) {
    if (object->field_7e != 0) {
        func_80128778(object);
    }
    func_80129be0(object);
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
