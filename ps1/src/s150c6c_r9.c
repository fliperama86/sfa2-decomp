/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to func_8011f240, which takes it: the original sets no argument register before that call, so the callee receives what this function's caller passed. No unit of the tree calls this function by name. */
void func_801523a4(Slab172 *p) {
    func_8011f240(p);
}

void func_801523c4(Object *object) {
    table_80180274[object->field_04](object);
}
