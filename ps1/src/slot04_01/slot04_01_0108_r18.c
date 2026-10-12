/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b36f8_slot04_01(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}
