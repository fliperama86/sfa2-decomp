/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3c70_slot04_0b(Object *o) {
    o->field_06++;
    o->field_0b ^= 1;
    func_801380f0(o);
    o->field_46 = 2;
    func_80131094(o);
}
