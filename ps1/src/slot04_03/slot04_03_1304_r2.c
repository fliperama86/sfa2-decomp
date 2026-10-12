/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_airborne_until_ground_landing_slot04_03(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->active_state = 1;
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        if (obj->field_49 != 0 && obj->vel_y > 0) {
            obj->accel_y = 0x10000;
        }
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->state_3++;
            obj->active_state = 0;
            obj->active_flag_4 = 0;
            obj->pos_y = (u16)obj->ground_y;
            play_object_sound_by_kind(obj);
            set_sequence_by_side_and_mode(obj, 0x28);
            return;
        }
    }
    update_sequence(obj);
}

void update_sequence_or_end_and_notify_other_slot04_03(Object *obj) {
    start_action_if_buttons_pressed(obj);
    if (!((s16)obj->step_flags & 0x8000)) {
        update_sequence(obj);
    } else {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
