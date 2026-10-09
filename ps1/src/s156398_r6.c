/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8018191c holds this function and is declared with this parameter. */
void func_80157360(Object *p) {
    func_8011f240((Slab172 *)p);
}
