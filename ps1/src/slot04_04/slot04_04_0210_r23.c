/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f14c(Slab172 *o);
extern void (*data_801c45bc_slot04_04[])(Object *);

void func_801b3f24_slot04_04(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801b3f44_slot04_04(Object *obj) {
    data_801c45bc_slot04_04[obj->field_04](obj);
}
