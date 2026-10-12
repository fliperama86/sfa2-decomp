/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b33c8_slot04_00(Object *obj) {
    obj->state_1 = obj->state_1 + 1;
    ref_other.p = obj->parent;
    ref_other.p->linked_object_3 = 0;
    func_80138070(obj, 6);
}
