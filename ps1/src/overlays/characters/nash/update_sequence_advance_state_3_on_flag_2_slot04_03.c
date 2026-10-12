/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_sequence_advance_state_3_on_flag_2_slot04_03(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3++;
    }
    update_sequence(obj);
}
