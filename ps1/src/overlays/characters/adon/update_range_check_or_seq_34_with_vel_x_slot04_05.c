/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_range_check_or_seq_34_with_vel_x_slot04_05(Object *obj) {
    int one;

    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x17, 0x11) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        } else if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            one = 1;
            obj->active_flag_3 = one;
            obj->state_3 = 2;
            obj->field_157 = 0;
            add_to_meter_clamped(obj, 1);
            obj->vel_x = 0x68000;
            obj->accel_x = -0x8000;
            reset_direction_and_action_fields(obj);
            obj->field_278 = one;
            obj->sequence_hold_flag = one;
            set_sequence_by_side_and_mode(obj, 0x34);
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
