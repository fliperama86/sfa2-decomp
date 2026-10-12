/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80142fbc(Object *object) {
    object->saved_facing = object->facing_side;
    if (object->active_state == 0) {
        object->vel_x = 0x20000;
        object->accel_x = 0xc000;
    }
}
