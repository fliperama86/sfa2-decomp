/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3d98_slot04_0b(Object *o, Object *unused) {
    o->state_1++;
    ref_other.p = o->parent;
    ref_other.p->linked_object_3 = 0;
    func_80138070(o, 9);
}
