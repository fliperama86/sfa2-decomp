/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077a18_slot00(Object *obj) {
    if ((((Slot00Obj *)obj->field_3c)->field_04 & 0xffffff) == 0x80001) {
        func_80120028(obj);
    } else {
        obj->field_04 = 3;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
}

void func_80077a78_slot00(Object *obj) {
    Object *p = obj->field_3c;
    p->field_246--;
    func_8011f14c((Slab172 *)obj);
}
