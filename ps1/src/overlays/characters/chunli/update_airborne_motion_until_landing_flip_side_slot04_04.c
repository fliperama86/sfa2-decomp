/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_new_vel_y_slot04_04(Object *obj);
void start_sequence_1c_1d_or_default_dir_sequence_slot04_04(Object *obj);
void set_sequence_by_direction_and_variant_slot04_04(Object *obj);

void update_airborne_motion_until_landing_flip_side_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (move_by_velocity_return_new_vel_y_slot04_04(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        if (obj->field_7e == 0) {
            obj->facing_side = obj->facing_side ^ 1;
        }
        play_object_sound_by_kind(obj);
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void set_air_direction_or_range_check_switch_slot04_04(Object *obj) {
    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    if (obj->input_dir_second != 0) {
        start_sequence_1c_1d_or_default_dir_sequence_slot04_04(obj);
    } else if (obj->direction_index != 0 && (obj->input_bits_2 & 0xe000) != 0 && obj->pos_y < obj->ground_y - 0x30 && (u8)check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        set_sequence_by_direction_and_variant_slot04_04(obj);
    }
}
