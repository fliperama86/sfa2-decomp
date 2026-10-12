/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_direction_sequence_or_start_move_slot04_03(Object *obj) {
    int t = 0xf;
    int one = 1;

    obj->state_3 = 3;
    obj->active_flag_3 = one;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        t = 0x15;
    }
    if (obj->input_dir_second == 0) {
        t -= 3;
        if (obj->direction_index != 0) {
            if (obj->ground_y - obj->pos_y >= 0x30 && (obj->input_bits_2 & 0xe000) != 0) {
                if (check_hit_other_active_state_nonzero(obj, -0x30, 0x10, 8, 0x10)) {
                    obj->state_0 = one;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    obj->input_dir_0 = one;
                    return;
                }
            }
        }
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + t));
}

