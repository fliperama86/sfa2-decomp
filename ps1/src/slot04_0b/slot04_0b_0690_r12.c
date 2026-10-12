/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_decrement_remaining_count_slot04_0b(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    set_sequence_by_side_and_mode(obj, 0x31);
}
