/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e00_slot04_0a(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;

    obj->field_10 += o->vel_x;
    o->vel_x += o->accel_x;
    obj->field_14 -= o->vel_y;
    o->vel_y += o->accel_y;
}
