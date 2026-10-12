/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_range_check_or_seq_1c_with_pos_x_shift_slot04_01(Object *obj) {
    int t;
    int x;

    x = obj->state_3;
    x++;
    obj->state_3 = x;
    x = obj->direction_index;
    t = 0x18;
    if (x != 0) {
        if (obj->field_25f == 0) {
            if ((obj->input_bits_2 & 0xa000) != 0) {
                if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
                    obj->state_0 = 1;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    return;
                }
                if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
                    obj->active_flag_3 = 1;
                    obj->state_3 = 2;
                    add_to_meter_clamped(obj, 1);
                    reset_direction_and_action_fields(obj);
                    obj->field_278 = 1;
                    obj->sequence_hold_flag = 1;
                    set_sequence_by_side_and_mode(obj, 0x1c);
                    return;
                }
            }
            if (obj->facing_side == 0) {
                obj->pos_x -= t;
            } else {
                obj->pos_x += t;
            }
        }
    }
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}
