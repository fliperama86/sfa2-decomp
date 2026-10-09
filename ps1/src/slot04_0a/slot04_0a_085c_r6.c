/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b42c4_slot04_0a(Object *obj) {
    ref_other.p = obj->field_3c;
    ref_other.p->field_240--;
    if (ref_other.p->field_240 & 0x80) {
        ref_other.p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)obj);
}
