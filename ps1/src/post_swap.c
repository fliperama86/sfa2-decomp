/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80132070(Object *object) {
    if ((*(u32 *)&object->field_04 & 0xffffff) == 0x70001 &&
        object->field_15a == 1 && object->field_7e == 0 &&
        object->field_67 != 0) {
        object->field_67 = 0;
        func_801307e0(object, 0x28);
    }
}

int func_801320e8(Object *object) {
    int result;
    if (object->field_cd != 0) {
        result = 0;
    } else if (!(u8)func_8013d0fc(object)) {
        result = 0;
    } else {
        result = (u8)func_801418bc(object) != 0;
    }
    return result;
}

void func_80132140(Object *object) {
    object->field_48 = 1;
    if (!(object->field_130 & 0x8000)) object->field_48 = 0xff;
    object->field_15a = 2;
    object->field_159 = 0;
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 7;
    object->field_07 = 0;
    func_80132198(object);
}

void func_80132198(Object *object) {
    u8 side = object->field_158;
    u16 bits = object->field_134 | object->field_136;
    u8 value;
    object->field_12c = 0;
    object->field_12d = 0;
    object->field_12e = 0;
    object->field_12f = 0;
    object->field_0b = side;
    if (bits & 0xc0) {
        value = 0;
    } else if (bits & 0x30) {
        value = 2;
    } else {
        value = 4;
    }
    object->field_12a = value;
}
