/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn table_80022e14_slot12[];

void func_80011ea4_slot12(Object *obj) {
    func_8011f240();
}

void func_80011ec4_slot12(Object *obj) {
    table_80022e14_slot12[obj->field_04](obj);
}
