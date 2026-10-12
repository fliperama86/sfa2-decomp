/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_directional_sequence_state_3_slot04_0a(Object *obj) {
    int t;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    t = 0xc;
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + t));
}
