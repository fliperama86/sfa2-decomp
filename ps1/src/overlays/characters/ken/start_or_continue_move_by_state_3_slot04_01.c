/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_or_continue_move_by_state_3_slot04_01(Object *obj) {
    obj->field_157 = 1;
    if (obj->state_3 == 0) {
        obj->active_flag_3 = 1;
        obj->state_3 = obj->state_3 + 1;
        set_sequence_and_tables_from_direction(obj);
    } else {
        update_sequence_with_input_check(obj);
    }
}

void set_direction_sequence_or_air_range_check_slot04_01(Object *obj) {
    int t = 0xc;
    int one = 1;

    obj->state_3 = 3;
    obj->active_flag_3 = one;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    } else if (obj->direction_index != 0) {
        if (obj->pos_y - obj->ground_y < -0x2f && (obj->input_bits_2 & 0xe000) != 0) {
            if (check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                obj->state_1 = 2;
                obj->state_0 = one;
                obj->state_2 = 0;
                obj->state_3 = 0;
                obj->input_dir_0 = 4;
                return;
            }
        }
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + t));
}
