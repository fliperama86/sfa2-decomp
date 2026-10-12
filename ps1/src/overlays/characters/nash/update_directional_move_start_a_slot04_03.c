/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_directional_move_start_a_slot04_03(Object *obj) {
    int one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x18, 0x10) != 0) {
            obj->state_0 = one;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            obj->input_dir_0 = 0;
        } else if (obj->direction_index == 4 && (obj->input_bits_2 & 0x8000) != 0) {
            add_to_meter_clamped(obj, 2);
            reset_direction_and_action_fields(obj);
            set_sequence_by_side_and_mode(obj, 0x1b);
        } else {
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        set_sequence_and_tables_from_direction(obj);
    }
}
