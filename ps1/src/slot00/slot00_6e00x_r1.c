/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80076e00_slot00(Object *o) {
    o->state_2++;
    o->facing_side ^= 1;
    func_801380f0(o);
    o->countdown = 2;
    func_80138070(o, o->variant_index_2);
}
