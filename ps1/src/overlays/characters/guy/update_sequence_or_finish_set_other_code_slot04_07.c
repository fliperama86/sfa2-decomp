/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_or_finish_set_other_code_slot04_07(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 6;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
