/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_with_velocity_until_vel_x_negative_slot04_05(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void apply_vertical_velocity_with_accel_override_slot04_05(Object *obj);

void update_airborne_until_landing_slot04_05(Object *obj) {
    apply_vertical_velocity_with_accel_override_slot04_05(obj);
    if (obj->ground_y >= obj->pos_y) {
        if (*(u8 *)&obj->step_flags != 3) {
            update_sequence(obj);
        }
    } else {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = (u16)obj->ground_y;
        update_sequence(obj);
    }
}
