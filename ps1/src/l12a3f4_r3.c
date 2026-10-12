/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_airborne_object(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object)) {
        if (object->mode_flag_2 != 0) {
            update_sequence_or_start_special_state(object);
        } else if (get_pressed_low_byte(object)) {
            call_scratch_fn_by_side_and_mode(object);
        } else if ((u8)is_pushing_toward_bound_in_air(object)) {
            start_wall_bounce_sequence(object);
        } else if (find_nearby_enemy_and_get_flag(object)) {
            set_state_3_4_and_sequence(object);
        } else {
            update_sequence(object);
        }
    } else {
        object->pos_y = object->ground_y;
        object->active_state = 0;
        object->active_flag_3 = 0;
        *(u32 *)&object->pos_y_frac &= 0xffff0000;
        play_object_sound_by_kind(object);
        if (object->mode_flag_2 == 0) {
            if (object->kind == 0xe && (u8)is_input_and_state_check_passed(object)) {
                set_state_7_variant_from_input(object);
            } else if (get_pressed_low_byte(object)) {
                set_state_1_0_5_and_read_input_direction(object);
            } else if (find_nearby_enemy_and_get_flag(object)) {
                enter_state_2_6_by_button_input(object);
            } else if ((u8)is_facing_differs_from_saved(object)) {
                set_win_state_or_state_4_sequence(object);
            } else {
                object->state_3 = object->state_3 + 1;
                set_sequence_by_side(object, 0x11);
            }
        } else {
            object->state_3 = object->state_3 + 1;
            set_sequence_by_side(object, 0x11);
        }
    }
}
