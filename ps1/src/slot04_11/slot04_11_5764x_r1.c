/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int apply_velocity_and_acceleration_slot04_11(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_x += obj->accel_x;
    return obj->vel_y += obj->accel_y;
}
