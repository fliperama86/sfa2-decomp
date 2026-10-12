/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void flip_facing_and_shift_position_on_step_flag_slot04_02(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->facing_side = obj->facing_side ^ 1;
        obj->pos_y = obj->pos_y + 0x27;
        if (obj->facing_side != 0) {
            obj->pos_x = obj->pos_x - 0x11;
        } else {
            obj->pos_x = obj->pos_x + 0x11;
        }
    }
}

void rise_until_apex_then_start_fall_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->vel_y = 0;
        obj->accel_x = 0;
        obj->vel_x = 0;
        obj->accel_y = -0x9000;
        set_sequence_by_side_and_mode(obj, 0x58);
    } else {
        update_sequence(obj);
    }
}
