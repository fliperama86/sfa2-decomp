/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

s32 apply_facing_velocity_with_x_stop_slot04_04(Object *object);

void update_air_motion_land_start_sequence_slot04_04(Object *obj) {
    if (apply_facing_velocity_with_x_stop_slot04_04(obj) >= 0) {
        update_sequence(obj);
    } else {
        if (obj->field_49 != 0) {
            obj->accel_y = 0xffff0000;
        } else {
            obj->accel_y = -0x6000;
        }
        if (obj->pos_y < obj->ground_y) {
            update_sequence(obj);
        } else {
            obj->state_3++;
            obj->pos_y_frac = 0;
            obj->active_state = 0;
            obj->active_flag_3 = 0;
            obj->active_flag_4 = 0;
            obj->pos_y = (u16)obj->ground_y;
            play_object_sound_by_kind(obj);
            set_sequence_by_side_and_mode(obj, obj->direction_index + 0x31);
        }
    }
}

void update_sequence_or_end_via_ref_other_2_slot04_04(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}
