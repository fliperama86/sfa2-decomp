/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_new_vel_y_slot04_07(Object *obj);
void move_x_by_facing_with_decel_stop_slot04_07(Object *obj);

void update_sequence_with_flag_driven_launch_phases_slot04_07(Object *obj) {
    s16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x8000) {
        obj->state_3 = 5;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x4c);
    } else if (t != 0x3ff) {
        if ((t & 0xff) != 0) {
            obj->step_flags = t & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        move_x_by_facing_with_decel_stop_slot04_07(obj);
    } else {
        obj->vel_y = 0x90000;
        obj->accel_y = 0xffff0000;
        obj->active_state = 1;
        obj->state_3++;
        obj->accel_x = 0;
        if (obj->facing_side != 0) {
            obj->vel_x = 0xfffc8000;
        } else {
            obj->vel_x = 0x38000;
        }
    }
}

void update_fall_until_ground_start_landing_slot04_07(Object *obj) {
    if (move_by_velocity_return_new_vel_y_slot04_07(obj) >= 0 || obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->state_3 = 5;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x4c);
    }
}

void update_sequence_set_x_velocity_on_flag_byte_slot04_07(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->vel_x = 0xe0000;
        obj->accel_x = -0x6000;
        obj->state_3++;
    }
    update_sequence(obj);
}
