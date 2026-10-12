/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_acceleration_slot04_0a(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;

    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
    obj->pos_y_hi -= o->vel_y;
    o->vel_y += o->accel_y;
}
