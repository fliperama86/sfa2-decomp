/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"

void func_80142fbc(Object *object) {
    object->field_158 = object->field_0b;
    if (object->field_45 == 0) {
        object->field_4c = 0x20000;
        object->field_54 = 0xc000;
    }
}
