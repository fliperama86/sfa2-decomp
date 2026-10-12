/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void advance_state_3_restore_facing_slot04_04(Object *obj);
void update_sequence_or_finish_on_flag_3_slot04_04(Object *obj);

void call_by_state_3_zero_restore_facing_or_update_slot04_04(Object *obj) {
    if (obj->state_3 == 0) {
        advance_state_3_restore_facing_slot04_04(obj);
    } else {
        update_sequence_or_finish_on_flag_3_slot04_04(obj);
    }
}
