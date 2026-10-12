/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_x_by_vel_x_facing_and_accelerate_slot04_05(Object *object);

void func_801b2c84_slot04_05(Object *obj) {
    s16 t = obj->step_flags;
    if (t & 0x8000) {
        obj->state_3++;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x40000;
        } else {
            obj->vel_x = 0xfffc0000;
        }
        obj->vel_y = 0x90000;
        obj->accel_y = -0x7000;
        obj->accel_x = 0;
        obj->active_state = 1;
        set_sequence_by_side_and_mode(obj, 0x2e);
    } else {
        switch ((u8)t) {
        case 0:
            break;
        default:
            if (obj->vel_x >= 0) {
                move_x_by_vel_x_facing_and_accelerate_slot04_05(obj);
            }
            break;
        case 3:
            obj->vel_x = 0x40000;
            obj->accel_x = 0xffff0000;
            break;
        }
        update_sequence(obj);
    }
}

void update_air_motion_until_landing_step_1_slot04_05(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if ((u8)obj->step_flags != 1) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
    }
}
