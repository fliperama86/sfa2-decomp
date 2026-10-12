/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj) {
    int t = obj->vel_x;

    if (obj->facing_side == 0) {
        t = -t;
    }
    *(s32 *)&obj->pos_x_frac += t;
    obj->vel_x += obj->accel_x;
}

void apply_vertical_velocity_slot04_06(Object *obj) {
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
}

void negate_value_if_facing_side_zero_slot04_06(Object *obj, u16 *p) {
    if (obj->facing_side == 0) {
        *p = -*p;
    }
}
