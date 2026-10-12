/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_xy_by_facing_with_x_decel_stop_slot04_07(Object *obj);
void set_state_3_seven_stop_with_gravity_sequence_slot04_07(Object *obj);
void update_motion_until_ground_state_4_slot04_07(Object *obj);

void update_motion_with_active_flag_branch_slot04_07(Object *obj) {
    move_xy_by_facing_with_x_decel_stop_slot04_07(obj);
    if (((Slot04aObj *)obj)->seen_flag == 0 && obj->active_flag != 0) {
        obj->vel_x = 0x10000;
        obj->vel_y = 0x10000;
        obj->accel_x = 0;
        obj->accel_y = 0;
        ((Slot04aObj *)obj)->seen_flag++;
    }
    if ((s16)obj->step_flags & 0x8000) {
        if (obj->active_flag == 0) {
            set_state_3_seven_stop_with_gravity_sequence_slot04_07(obj);
            return;
        }
        obj->state_3 = 5;
        obj->countdown += 2;
    }
    update_sequence(obj);
}

void update_motion_until_ground_state_4_slot04_07(Object *obj) {
    int t;

    move_xy_by_facing_with_x_decel_stop_slot04_07(obj);
    t = obj->ground_y;
    if (obj->pos_y < t) {
        update_sequence(obj);
    } else {
        obj->state_3 = 4;
        obj->pos_y = t;
        obj->active_state = 0;
        set_sequence_by_side_and_mode(obj, 0x2e);
    }
}

void move_xy_by_facing_with_x_decel_stop_slot04_07(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}
