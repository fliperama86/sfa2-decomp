/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_state_3_and_sequence_from_direction_slot04_0b(Object *obj) {
    int x = 0xc;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        x = 0x12;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + x));
}
