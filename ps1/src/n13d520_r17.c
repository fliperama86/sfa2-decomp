/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144d88(Object *object) {
    if ((u8)object->field_3a != 0) {
        object->field_46 = 0x1f;
        object->field_4c = 0x20000;
        object->field_54 = 0x8000;
        object->field_06 = object->field_06 + 1;
        object->field_3a = object->field_3a & 0xff00;
    }
}
