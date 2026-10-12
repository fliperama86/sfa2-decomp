/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_vel_accel_x_on_step_flag_then_update_slot04_03(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x50000;
            obj->accel_x = -0x2000;
        } else {
            obj->vel_x = -0x50000;
            obj->accel_x = 0x2000;
        }
    } else if ((t & 0xff00) == 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
    }
    update_sequence(obj);
}

void move_x_with_accel_and_step_on_flag_slot04_03(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    update_sequence(obj);
}

void update_sequence_step_state_3_on_flag_slot04_03(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
    }
}
