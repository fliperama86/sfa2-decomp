/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_or_end_set_other_code_slot04_06(Object *object);

void start_sequence_3c_and_decrement_remaining_slot04_06(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->field_157 = 0;
    set_sequence_by_side_and_mode(obj, 0x3c);
}

void update_sequence_or_finish_on_status_bit_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    if (obj->status_flags & 0x80) {
        update_sequence_or_end_set_other_code_slot04_06(o);
    } else {
        update_sequence(o);
    }
}
