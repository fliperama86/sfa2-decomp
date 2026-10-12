/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_direction_sequence_or_hit_air_range_check_slot04_03(Object *obj) {
    int k = 0xc;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    if (obj->hit_flag_a != 0 && obj->ground_y - obj->pos_y >= 0x30 && check_hit_other_active_state_nonzero(obj, -0x30, 0x10, 0x20, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->input_dir_0 = 1;
    } else {
        add_to_meter_clamped(obj, obj->direction_index >> 1);
        if (obj->variant_index != 0) {
            k = 0x12;
        }
        if (obj->input_dir_second != 0) {
            k += 3;
        }
        set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + k));
    }
}
