/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_state_3_on_negative_flags_advance_sequence_slot04_0b(Object *obj, Object *unused) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}
