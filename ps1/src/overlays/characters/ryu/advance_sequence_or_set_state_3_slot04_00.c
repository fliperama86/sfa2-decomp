/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void advance_sequence_or_set_state_3_slot04_00(Object *o) {
    if ((s16)o->step_flags >= 0) {
        advance_sequence_checked(o);
    } else {
        o->state_0 = 3;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
    }
}
