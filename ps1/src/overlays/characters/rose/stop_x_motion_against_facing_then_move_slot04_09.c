/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);

void stop_x_motion_against_facing_then_move_slot04_09(Object *obj) {
    if (obj->vel_x >= 0) {
        if (obj->facing_side == 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    } else {
        if (obj->facing_side != 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    }
    apply_velocity_and_acceleration_slot04_09(obj);
}
