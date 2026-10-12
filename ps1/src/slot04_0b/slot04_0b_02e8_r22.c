/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3c70_slot04_0b(Object *o) {
    o->state_2++;
    o->facing_side ^= 1;
    func_801380f0(o);
    o->countdown = 2;
    advance_sequence_checked(o);
}
