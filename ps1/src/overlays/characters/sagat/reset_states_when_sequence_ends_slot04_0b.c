/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void noop_slot00_slot04_0b(Object *obj);

void reset_states_when_sequence_ends_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        noop_slot00_slot04_0b(obj);
        set_sequence_by_side(obj, 0);
    } else {
        update_sequence(obj);
    }
}
