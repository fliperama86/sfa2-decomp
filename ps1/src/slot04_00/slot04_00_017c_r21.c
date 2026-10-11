/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b340c_slot04_00(Object *o) {
    if ((s16)o->step_flags >= 0) {
        func_80131094(o);
    } else {
        o->state_0 = 3;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
    }
}
