/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_vel_y_with_accel_y_override_slot04_00(Object *obj);
void land_on_ground_and_update_sequence_slot04_00(Object *obj);

void update_airborne_motion_or_land_slot04_00(Object *obj) {
    apply_vel_y_with_accel_y_override_slot04_00(obj);
    if (obj->pos_y >= obj->ground_y) {
        land_on_ground_and_update_sequence_slot04_00(obj);
    } else {
        if (obj->facing_side == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->state_3++;
        }
        update_sequence(obj);
    }
}

void update_vertical_fall_until_ground_slot04_00(Object *obj) {
    apply_vel_y_with_accel_y_override_slot04_00(obj);
    if (obj->pos_y >= obj->ground_y) {
        land_on_ground_and_update_sequence_slot04_00(obj);
    } else if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    }
}

void land_on_ground_and_update_sequence_slot04_00(Object *obj) {
    obj->state_3++;
    obj->active_state = 0;
    obj->active_flag_3 = 0;
    obj->active_flag_4 = 0;
    obj->pos_y = (u16)obj->ground_y;
    play_object_sound_by_kind(obj);
    update_sequence(obj);
}
