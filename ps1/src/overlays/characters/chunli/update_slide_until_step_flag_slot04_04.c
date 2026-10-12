/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_slide_until_step_flag_slot04_04(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 0;
        obj->state_3++;
    } else {
        if (obj->vel_x < 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
    }
    update_sequence(obj);
}

void copy_vel_y_to_vel_x_on_step_end_slot04_04(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xb);
        obj->vel_x = obj->vel_y;
        obj->accel_x = obj->accel_y;
    }
    update_sequence(obj);
}

void update_sequence_or_finish_and_reset_other_code_slot04_04(Object *obj) {
    if (!((s16)obj->step_flags & 0x8000)) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->vel_y = 0;
        obj->accel_y = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
