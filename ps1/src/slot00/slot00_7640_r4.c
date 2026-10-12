/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077a18_slot00(Object *obj) {
    if ((((Slot00Obj *)obj->parent)->state_key & 0xffffff) == 0x80001) {
        func_80120028(obj);
    } else {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}

void func_80077a78_slot00(Object *obj) {
    Object *p = obj->parent;
    p->counter--;
    func_8011f14c((Slab172 *)obj);
}
