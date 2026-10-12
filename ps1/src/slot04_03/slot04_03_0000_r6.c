/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_airborne_motion_until_landing_slot04_03(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        update_sequence(obj);
        reset_direction_and_action_fields(obj);
        set_sequence_by_side_and_mode(obj, 0x1e);
    }
}

void update_sequence_or_end_with_other_code_slot04_03(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 8;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
