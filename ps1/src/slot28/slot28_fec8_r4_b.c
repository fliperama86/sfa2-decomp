/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_accel_slot28(Object *o);

void apply_velocity_and_accel_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;

    obj->pos_x += o->vel_x;
    o->vel_x += o->accel_x;
    obj->pos_y -= o->vel_y;
    o->vel_y += o->accel_y;
}
