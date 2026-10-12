/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_sequence_from_direction_and_variant_b_slot04_06(Object *obj) {
    u16 x;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    x = 0x12;
    if (obj->variant_index == 0) {
        x = 0xc;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    x += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, x);
}
