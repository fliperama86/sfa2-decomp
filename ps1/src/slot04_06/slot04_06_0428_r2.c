/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2610_slot04_06(Object *o) {
    if (*(u8 *)&o->step_flags != 0) {
        o->field_45 = 1;
        ((Slot04aObj *)o)->field_1c8 = 0;
        o->state_3++;
    }
    update_sequence(o);
}
