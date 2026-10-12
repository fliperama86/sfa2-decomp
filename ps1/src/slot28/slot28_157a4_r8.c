/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void ignore_object_empty_37_slot28(Object *obj) {
}

void advance_state_0_on_negative_step_flags_2_slot28(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}
