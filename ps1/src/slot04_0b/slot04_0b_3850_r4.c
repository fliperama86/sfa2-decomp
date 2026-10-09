/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3d98_slot04_0b(Object *o, Object *unused) {
    o->field_05++;
    ref_other.p = o->field_3c;
    ref_other.p->field_14c = 0;
    func_80138070(o, 9);
}
