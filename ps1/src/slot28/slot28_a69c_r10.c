/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void integrate_position_and_ground_y_motion_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->pos_x += o->vel_x;
    o->vel_x += o->accel_x;
    obj->pos_y -= o->vel_y;
    o->vel_y += o->accel_y;
    o->ground_y -= o->health;
    o->health += o->y_offset;
}
