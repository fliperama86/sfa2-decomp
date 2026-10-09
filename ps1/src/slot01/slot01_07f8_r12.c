/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015a50_slot01[];

void func_800136d4_slot01(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_800136f4_slot01(Object *object) {
    data_80015a50_slot01[object->field_04](object);
}
