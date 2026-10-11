/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80142fbc(Object *object) {
    object->field_158 = object->field_0b;
    if (object->field_45 == 0) {
        object->vel_x = 0x20000;
        object->accel_x = 0xc000;
    }
}
