/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void on_negative_step_flags_next_state_0(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->ot_depth = 0;
        object->state_0 = object->state_0 + 1;
    }
}
