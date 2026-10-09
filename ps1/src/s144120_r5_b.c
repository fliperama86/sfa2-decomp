/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801448c4(Object *object) {
    if ((s16)object->field_3a < 0) {
        object->pos_x = 0xc0;
        object->field_06 = object->field_06 + 1;
        func_80144ee4(object, 0x10);
        func_80144938(object);
        object->field_01 = 1;
        object->field_7a = 0x80;
        object->field_09 = 1;
    } else {
        object->field_01 = 2;
    }
}

void func_80144938(Object *object) {
    if (object->field_3a & 0x80) {
        object->field_46 = 7;
        object->field_4c = 0x8000;
        object->field_50 = -0x10000;
        object->field_06 = object->field_06 + 1;
        object->field_3a = object->field_3a & 0xff00;
    }
}

void func_80144980(Object *object) {
    func_80144244(object);
    object->field_46 = (s16)object->field_46 - 1;
    if (object->field_46 & 0x80) {
        object->field_06 = object->field_06 + 1;
    }
}
