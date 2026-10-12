/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_46_or_47_by_input_slot04_0a(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    a = 0x46;
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    if (obj->input_bits_2 & 0x8000) {
        a = 0x47;
    }
    set_sequence_by_side_and_mode(obj, a);
}
