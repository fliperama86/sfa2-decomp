/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void select_transition_at_sequence_end(Object *object) {
    if (object->mode_flag_2 != 0) {
        if ((s16)object->step_flags < 0)
            reset_object_to_state_1_0_sequence_0(object);
        else
            update_sequence(object);
    } else if (get_pressed_low_byte(object)) {
        set_state_1_0_5_and_read_input_direction(object);
    } else if ((u8)is_facing_differs_from_saved(object)) {
        set_win_state_or_state_4_sequence(object);
    } else if (find_nearby_enemy_and_get_flag(object)) {
        enter_state_2_6_by_button_input(object);
    } else if ((s16)object->step_flags >= 0) {
        update_sequence(object);
    } else if ((u8)is_button_4000_held_or_other_flag(object)) {
        set_state_1_0_1_0_sequence_7(object);
    } else {
        reset_object_to_state_1_0_sequence_0(object);
    }
}

void update_airborne_motion_with_kind_effects(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object)) {
        if ((s16)object->step_flags < 0 && object->field_7e != 0) {
            object->state_3 = 1;
            object->active_flag_3 = 0;
        }
        update_sequence(object);
        if (object->kind == 0xb) {
            if ((object->step_flags & 0xff) != 0) {
                object->step_flags = object->step_flags & 0xff00;
                play_sound_from_table_by_direction_index(object);
            }
        } else if (object->kind == 0x14) {
            enter_state_7_if_airborne_conditions(object);
        }
    } else {
        object->pos_y = object->ground_y;
        object->active_state = 0;
        object->active_flag_3 = 0;
        play_object_sound_by_kind(object);
        if (object->mode_flag_2 == 0 && find_nearby_enemy_and_get_flag(object)) {
            enter_state_2_6_by_button_input(object);
        } else {
            object->state_3 = 2;
            set_sequence_by_side(object, 0x11);
        }
    }
}

void update_airborne_motion_or_land(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object)) {
        if (object->mode_flag_2 != 0) {
            update_sequence_or_set_state_3_to_1(object);
        } else if (get_pressed_low_byte(object)) {
            call_scratch_fn_by_side_and_mode(object);
        } else {
            if (find_nearby_enemy_and_get_flag(object) == 0)
                object->state_3 = 1;
            update_sequence(object);
        }
    } else {
        object->pos_y = object->ground_y;
        object->active_state = 0;
        object->active_flag_3 = 0;
        play_object_sound_by_kind(object);
        if (object->mode_flag_2 == 0 && find_nearby_enemy_and_get_flag(object)) {
            enter_state_2_6_by_button_input(object);
        } else {
            object->state_3 = 2;
            set_sequence_by_side(object, 0x11);
        }
    }
}
