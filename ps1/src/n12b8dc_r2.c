/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012de9c(Object *object) {
    int v;
    object->field_48 = 0xff;
    object->field_45 = 0xff;
    object->vel_y = 0x10000;
    object->accel_y = -0x6800;
    object->field_46 = 0;
    object->accel_x = 0;
    object->state_3 = object->state_3 + 1;
    if (object->field_72 == 0) {
        v = -0x60000;
    } else {
        v = 0x60000;
    }
    object->vel_x = v;
}
