/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e1c_slot04_0b(Object *o, Object *unused) {
    ref_other.p = o->field_3c;
    ref_other.p->field_240--;
    if (ref_other.p->field_240 == 0) {
        ref_other.p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}
