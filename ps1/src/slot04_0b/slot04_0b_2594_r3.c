/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c260c_slot04_0b[];

void launch_from_velocity_table_on_flag_6_slot04_0b(Object *obj) {
    int k;

    if ((u8)obj->step_flags == 6) {
        obj->state_3++;
        k = (obj->direction_index >> 1) * 3 + obj->variant_index_2;
        obj->vel_x = data_801c260c_slot04_0b[k];
        obj->vel_y = data_801c260c_slot04_0b[k + 1];
        obj->accel_x = -0x8000;
        obj->accel_y = data_801c260c_slot04_0b[k + 2];
        obj->active_state = 1;
    }
    update_sequence(obj);
}

void move_arc_until_vel_x_negative_slot04_0b(Object *obj) {
    if (obj->vel_x >= 0) {
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->facing_side == 0) {
            *(s32 *)&obj->pos_x_frac -= obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
        }
        obj->vel_x += obj->accel_x;
    } else {
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->state_3++;
    }
    update_sequence(obj);
}
