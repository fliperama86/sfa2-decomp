/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_3e_or_3f_by_input_bit_slot04_03(Object *obj) {
    int a = 0x3e;

    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    if (obj->input_bits_2 & 0x8000) {
        a = 0x3f;
    }
    set_sequence_by_side_and_mode(obj, a);
}
