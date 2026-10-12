/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012abf0(Object *object) {
    if (object->field_cd != 0) {
        if ((s16)object->step_flags < 0)
            func_801312b8(object);
        else
            update_sequence(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_8012f898(object)) {
        func_8012f8c4(object);
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if ((s16)object->step_flags >= 0) {
        update_sequence(object);
    } else if ((u8)func_8012f618(object)) {
        func_8012f6a0(object);
    } else {
        func_801312b8(object);
    }
}

void func_8012acf4(Object *object) {
    if ((u8)func_80130184(object)) {
        if ((s16)object->step_flags < 0 && object->field_7e != 0) {
            object->state_3 = 1;
            object->field_159 = 0;
        }
        update_sequence(object);
        if (object->kind == 0xb) {
            if ((object->step_flags & 0xff) != 0) {
                object->step_flags = object->step_flags & 0xff00;
                func_80120af8(object);
            }
        } else if (object->kind == 0x14) {
            func_80132370(object);
        }
    } else {
        object->pos_y = object->ground_y;
        object->field_45 = 0;
        object->field_159 = 0;
        func_801209c4(object);
        if (object->field_cd == 0 && func_8012f970(object)) {
            func_8012fe60(object);
        } else {
            object->state_3 = 2;
            func_80130678(object, 0x11);
        }
    }
}

void func_8012ae00(Object *object) {
    if ((u8)func_80130184(object)) {
        if (object->field_cd != 0) {
            func_8012aef8(object);
        } else if (func_80130470(object)) {
            func_80130304(object);
        } else {
            if (func_8012f970(object) == 0)
                object->state_3 = 1;
            update_sequence(object);
        }
    } else {
        object->pos_y = object->ground_y;
        object->field_45 = 0;
        object->field_159 = 0;
        func_801209c4(object);
        if (object->field_cd == 0 && func_8012f970(object)) {
            func_8012fe60(object);
        } else {
            object->state_3 = 2;
            func_80130678(object, 0x11);
        }
    }
}
