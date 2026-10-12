/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001b1c4_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_10 += o->vel_x;
    o->vel_x += o->accel_x;
    obj->field_14 -= o->vel_y;
    o->vel_y += o->accel_y;
    o->ground_y -= o->health;
    o->health += o->field_5e;
}
