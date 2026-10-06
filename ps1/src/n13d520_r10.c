/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80143f98(Object *object) {
    if ((u8)object->field_3a != 0) {
        object->field_46 = 7;
        object->field_4c = -0x8000;
        object->field_50 = 0x10000;
        object->field_06 = object->field_06 + 1;
        object->field_3a = object->field_3a & 0xff00;
    }
}
