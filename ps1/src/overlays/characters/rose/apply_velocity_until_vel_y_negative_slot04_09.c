/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);

void apply_velocity_until_vel_y_negative_slot04_09(Object *obj) {
    apply_velocity_and_acceleration_slot04_09(obj);
    if (obj->vel_y < 0) {
        obj->state_3++;
        obj->accel_y = obj->accel_y * 3;
        obj->facing_side = obj->facing_side ^ 1;
        set_sequence_by_side_and_mode(obj, 0x4a);
    }
}

void update_air_state_until_landing_slot04_09(Object *obj) {
    apply_velocity_and_acceleration_slot04_09(obj);
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->active_flag_4 = 0;
        obj->pos_y = obj->ground_y;
        start_action_if_buttons_pressed(obj);
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x4b);
    }
}

void update_sequence_or_call_end_handler_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        obj->active_flag_4 = 0;
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}
