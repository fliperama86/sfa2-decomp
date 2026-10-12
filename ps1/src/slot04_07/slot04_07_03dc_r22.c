/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_3_restore_facing_set_sequence_slot04_07(Object *obj);
void update_sequence_or_finish_on_flag_2_slot04_07(Object *obj);

void call_handler_by_state_3_slot04_07(Object *obj) {
    if (obj->state_3 == 0) {
        advance_state_3_restore_facing_set_sequence_slot04_07(obj);
    } else {
        update_sequence_or_finish_on_flag_2_slot04_07(obj);
    }
}
