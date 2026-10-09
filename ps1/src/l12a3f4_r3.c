/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012a9a0(Object *object) {
    if ((u8)func_80130184(object)) {
        if (object->field_cd != 0) {
            func_8012ab5c(object);
        } else if (func_80130470(object)) {
            func_80130304(object);
        } else if ((u8)func_8012f2b8(object)) {
            func_8012f384(object);
        } else if (func_8012f970(object)) {
            func_8012f3e0(object);
        } else {
            func_80130efc(object);
        }
    } else {
        object->pos_y = object->field_70;
        object->field_45 = 0;
        object->field_159 = 0;
        *(u32 *)&object->field_14 &= 0xffff0000;
        func_801209c4(object);
        if (object->field_cd == 0) {
            if (object->kind == 0xe && (u8)func_801320e8(object)) {
                func_80132140(object);
            } else if (func_80130470(object)) {
                func_8013047c(object);
            } else if (func_8012f970(object)) {
                func_8012fe60(object);
            } else if ((u8)func_8012f898(object)) {
                func_8012f8c4(object);
            } else {
                object->field_07 = object->field_07 + 1;
                func_80130678(object, 0x11);
            }
        } else {
            object->field_07 = object->field_07 + 1;
            func_80130678(object, 0x11);
        }
    }
}
