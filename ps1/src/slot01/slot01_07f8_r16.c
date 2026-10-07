/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011ffdc(Object *obj);
void func_8011f240(Slab172 *s);
extern ObjectFn data_80015dc0_slot01[];

void func_80014714_slot01(Object *obj) {
    func_8011ffdc(obj);
}

void func_80014734_slot01(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80014754_slot01(Object *object) {
    data_80015dc0_slot01[object->field_04](object);
}
