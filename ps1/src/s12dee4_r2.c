/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012e00c(Object *object) {
    handlers_1a6c[object->field_07](object);
}

void func_8012e04c(Object *object) {
    int value = object->field_50;
    int a1;

    if ((u8)func_80130184(object)) {
        if (value < 0) {
            func_80130efc(object);
        }
    } else {
        a1 = 1;
        value = -0x60000;
        object->field_60 = 2;
        object->field_07 = 7;
        object->field_46 = 4;
        object->field_45 = 0;
        object->field_166 = 1;
        object->pos_y = (u16)object->field_70;
        if (object->field_4c >= 0) {
            a1 = 0;
            value = 0x60000;
        }
        object->field_0b = a1;
        object->field_4c = value;
        func_801308c4(object, 0x1f);
    }
}

void func_8012e0fc(Object *object) {
    handlers_1a74[object->field_07](object);
}

void func_8012e13c(Object *object) {
    object->field_07 = object->field_07 + 1;
    func_8012e168(object);
}
