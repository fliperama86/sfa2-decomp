/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80144b98(Object *object) {
    if ((u8)object->step_flags != 0) {
        object->countdown = 0xf;
        object->vel_x = 0xc0000;
        object->accel_x = 0x8000;
        object->state_2 = object->state_2 + 1;
        object->step_flags = object->step_flags & 0xff00;
    }
}
