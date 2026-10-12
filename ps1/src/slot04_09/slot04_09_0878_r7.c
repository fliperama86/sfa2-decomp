/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_or_finish_clearing_flag_4_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->active_flag_4 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
