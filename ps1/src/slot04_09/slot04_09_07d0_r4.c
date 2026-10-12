/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enter_state_3_set_sequence_by_direction_slot04_09(Object *obj) {
    int t = 0xc;
    int s = 1;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    obj->field_225 = 1;
    if (obj->counter == 0) {
        obj->field_225 = 0;
        s = obj->direction_index >> 1;
        add_to_meter_clamped(obj, s);
    }
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    set_sequence_by_side_and_mode(obj, (s16)(t + s));
}
