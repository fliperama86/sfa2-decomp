/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b31f4_slot04_04(Object *o) {
    if (((s16)o->step_flags & 0x8000) == 0) {
        update_sequence(o);
    } else {
        ref_other.p = o->other;
        ref_other.p->other_state_code = 5;
        func_801312b8(o);
    }
}
