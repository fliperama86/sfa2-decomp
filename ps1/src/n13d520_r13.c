/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144500(Object *object) {
    if ((u8)object->field_3a != 0) {
        object->field_46 = 0xf;
        object->field_4c = 0xfff40000;
        object->field_54 = 0xfff80000;
        object->field_06 = object->field_06 + 1;
        object->field_3a = object->field_3a & 0xff00;
    }
}
