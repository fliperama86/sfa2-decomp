/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_range_check_or_seq_1c_or_1d_jump_slot04_03(Object *obj) {
    int one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x10) != 0) {
            obj->state_0 = one;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            obj->input_dir_0 = 2;
        } else if (obj->direction_index != 2) {
            if ((obj->input_bits_2 & 0xa000) != 0) {
                obj->state_3 = 2;
                *(s16 *)&obj->countdown = -1;
                obj->variant_index = obj->facing_side ^ 1;
                add_to_meter_clamped(obj, 2);
                reset_direction_and_action_fields(obj);
                obj->field_278 = one;
                set_sequence_by_side_and_mode(obj, 0x1c);
            } else {
                set_sequence_and_tables_from_direction(obj);
            }
        } else if ((obj->input_bits_2 & 0xa000) != 0 && obj->field_262 != 0) {
            obj->vel_y = -0x40000;
            obj->accel_y = 0x6000;
            if (obj->facing_side != 0) {
                obj->vel_x = 0x36000;
            } else {
                obj->vel_x = -0x36000;
            }
            obj->pos_y_frac = 0;
            obj->state_3 = 3;
            add_to_meter_clamped(obj, 1);
            reset_direction_and_action_fields(obj);
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
            set_sequence_by_side_and_mode(obj, 0x1d);
        } else {
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}
