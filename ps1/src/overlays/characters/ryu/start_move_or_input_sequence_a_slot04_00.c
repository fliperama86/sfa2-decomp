/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_move_or_input_sequence_a_slot04_00(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        } else if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            obj->field_157 = 0;
            add_to_meter_clamped(obj, 1);
            reset_direction_and_action_fields(obj);
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
            set_sequence_by_side_and_mode(obj, 0x23);
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
