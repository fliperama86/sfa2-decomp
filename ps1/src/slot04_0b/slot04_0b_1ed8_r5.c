/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2460_slot04_0b(Object *o) {
    if ((s16)o->step_flags & 0x8000) {
        o->state_3++;
        o->field_45 = 0;
        func_801209c4(o);
        func_801307e0(o, 0x30);
    } else {
        update_sequence(o);
    }
}
