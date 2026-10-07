/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80076e00_slot00(Object *o) {
    o->field_06++;
    o->field_0b ^= 1;
    func_801380f0(o);
    o->field_46 = 2;
    func_80138070(o, o->field_a0);
}
