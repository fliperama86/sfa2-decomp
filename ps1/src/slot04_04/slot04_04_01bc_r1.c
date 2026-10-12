/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void reset_states_when_step_flag_negative_slot04_04(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        set_sequence_by_side(obj, 0);
    } else {
        update_sequence(obj);
    }
}
