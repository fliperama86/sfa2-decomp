/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80143fdc(Object *object) {
    func_80144244(object);
    object->field_46 = (s16)object->field_46 - 1;
    if (object->field_46 & 0x80) object->field_06 = object->field_06 + 1;
}

void func_80144030(Object *object) {
    if ((s16)object->field_3a < 0) {
        if ((u8)func_801441c8(object) == 0) func_80144e90(object);
        else func_80144090(object);
    }
}

void func_80144090(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->field_46 = 0xf;
    object->field_4c = 0xfff40000;
    object->field_54 = -0x8000;
    object->field_06 = object->field_06 + 1;
    func_80144ee4(object, 0xc);
}
