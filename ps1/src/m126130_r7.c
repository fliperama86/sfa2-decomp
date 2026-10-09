/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8012f898(Object *object);

void func_8012abf0(Object *object) {
    if (object->field_cd != 0) {
        if ((s16)object->field_3a < 0)
            func_801312b8(object);
        else
            func_80130efc(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_8012f898(object)) {
        func_8012f8c4(object);
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if ((s16)object->field_3a >= 0) {
        func_80130efc(object);
    } else if ((u8)func_8012f618(object)) {
        func_8012f6a0(object);
    } else {
        func_801312b8(object);
    }
}

void func_8012acf4(Object *object) {
    if ((u8)func_80130184(object)) {
        if ((s16)object->field_3a < 0 && object->field_7e != 0) {
            object->field_07 = 1;
            object->field_159 = 0;
        }
        func_80130efc(object);
        if (object->kind == 0xb) {
            if ((object->field_3a & 0xff) != 0) {
                object->field_3a = object->field_3a & 0xff00;
                func_80120af8(object);
            }
        } else if (object->kind == 0x14) {
            func_80132370(object);
        }
    } else {
        object->pos_y = object->field_70;
        object->field_45 = 0;
        object->field_159 = 0;
        func_801209c4(object);
        if (object->field_cd == 0 && func_8012f970(object)) {
            func_8012fe60(object);
        } else {
            object->field_07 = 2;
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
                object->field_07 = 1;
            func_80130efc(object);
        }
    } else {
        object->pos_y = object->field_70;
        object->field_45 = 0;
        object->field_159 = 0;
        func_801209c4(object);
        if (object->field_cd == 0 && func_8012f970(object)) {
            func_8012fe60(object);
        } else {
            object->field_07 = 2;
            func_80130678(object, 0x11);
        }
    }
}
