/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8018023c holds this function and is declared with this parameter. */
void func_801523a4(Object *p) {
    func_8011f240((Slab172 *)p);
}

void func_801523c4(Block172 *block) {
    Object *object = (Object *)block;
    table_80180274[object->field_04](object);
}
