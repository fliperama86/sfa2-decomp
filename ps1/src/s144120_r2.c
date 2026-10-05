/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80144364(Object *object) {
    if (*(u8 *)&object->field_3a != 0) {
        object->field_06 = object->field_06 + 1;
        object->field_3a = object->field_3a & 0xff00;
        func_80120554(0, 0, 0x20b);
    }
}
