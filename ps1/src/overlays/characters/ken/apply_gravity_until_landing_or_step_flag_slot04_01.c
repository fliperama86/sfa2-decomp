/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_gravity_until_landing_or_step_flag_slot04_01(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
    }
    update_sequence(obj);
}

void finish_sequence_or_set_other_state_5_3_slot04_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
