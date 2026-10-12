/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s16 data_801c45d8_slot04_04[];
void set_state_5_and_start_variant_sequence_slot04_04(Object *obj);

void select_attack_sequence_by_direction_slot04_04(Object *obj) {
    int t = 0xc;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    if (obj->input_dir_second == 0) {
        if (obj->direction_index != 0) {
            if (obj->hit_flag_a != 0 && obj->ground_y - 0x30 < obj->pos_y) {
                if (check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                    obj->state_0 = 1;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    return;
                }
            }
        }
    } else if (obj->direction_index == 2 && obj->attack_param_d != 0) {
        set_state_5_and_start_variant_sequence_slot04_04(obj);
        return;
    }
    add_to_meter_clamped(obj, data_801c45d8_slot04_04[obj->direction_index >> 1]);
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(obj, (u16)((obj->direction_index >> 1) + t));
}
