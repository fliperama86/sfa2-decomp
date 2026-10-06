/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079ff8_slot00[];
void func_8011f14c(Slab172 *o);
void func_800776a8_slot00(Object *obj, short delta);

void func_80077640_slot00(Object *obj) {
    Object *p = obj->field_3c;
    obj->field_04++;
    func_800776a8_slot00(obj, 5);
    func_801204f4(p, p->side, 0x14);
}

void func_80077688_slot00(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_800776a8_slot00(Object *obj, short delta) {
    Object *p = obj->field_3c;
    if (p->field_246 == 0) {
        func_80141f28(p, delta);
    }
}

void func_800776e0_slot00(Object *obj) {
    data_80079ff8_slot00[obj->field_04](obj);
}
