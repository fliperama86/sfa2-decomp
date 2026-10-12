/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_air_motion_land_and_set_bounce_a_2_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y = obj->accel_y - 0x2000;
    }
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if (*(u8 *)&obj->step_flags != 2) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        obj->pos_y = obj->ground_y;
        obj->active_state = 0;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x70000;
        } else {
            obj->vel_x = -0x70000;
        }
        obj->vel_y = 0x30000;
        obj->accel_x = 0;
        obj->accel_y = -0x6000;
    }
}

void update_fall_until_landing_extra_gravity_2_slot04_05(Object *obj) {
    if (obj->vel_y < 0) {
        obj->accel_y -= 0x3000;
    }
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if ((u8)obj->step_flags != 2) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
    }
}
