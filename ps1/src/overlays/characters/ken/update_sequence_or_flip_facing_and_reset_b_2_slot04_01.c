/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_or_flip_facing_and_reset_b_2_slot04_01(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
