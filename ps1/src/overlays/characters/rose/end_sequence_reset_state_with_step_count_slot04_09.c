/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void end_sequence_reset_state_with_step_count_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->active_flag_3 = 0;
        obj->state_3 = 0;
        obj->step_count = 0;
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        update_sequence(obj);
    }
}
