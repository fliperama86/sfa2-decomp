/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void update_range_check_or_jump_seq_40_with_hold_slot04_02(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
            set_object_states_0_to_3_slot04_02(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0) {
            obj->active_flag_3 = one;
            obj->state_3 = 2;
            obj->field_157 = 0;
            add_to_meter_clamped(obj, 1);
            if (obj->facing_side != 0) {
                obj->vel_x = 0x36000;
            } else {
                obj->vel_x = -0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            reset_direction_and_action_fields(obj);
            obj->sequence_hold_flag = one;
            obj->field_278 = one;
            set_sequence_by_side_and_mode(obj, 0x40);
            return;
        }
    }
    obj->active_flag_3 = one;
    set_sequence_and_tables_from_direction(obj);
}
