/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_3_on_step_flag_byte_2_slot04_06(Object *o) {
    if (*(u8 *)&o->step_flags != 0) {
        o->active_state = 1;
        ((Slot04aObj *)o)->field_1c8 = 0;
        o->state_3++;
    }
    update_sequence(o);
}
