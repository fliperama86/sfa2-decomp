/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_vertical_motion_with_fall_accel_slot04_0b(Object *obj);

void move_x_by_vel_x_until_negative_slot04_0b(Object *obj) {
    if (obj->vel_x >= 0) {
        apply_vertical_motion_with_fall_accel_slot04_0b(obj);
        if (obj->facing_side == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    } else {
        obj->state_3++;
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    update_sequence(obj);
}

void update_airborne_until_ground_landing_slot04_0b(Object *obj) {
    int a;

    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    apply_vertical_motion_with_fall_accel_slot04_0b(obj);
    a = 0x29;
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        obj->active_flag_4 = 0;
        if (obj->direction_index != 0) {
            a = 0x2a;
        }
        set_sequence_by_side_and_mode(obj, a);
    } else {
        update_sequence(obj);
    }
}
