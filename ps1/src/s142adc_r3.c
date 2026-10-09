/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801436dc(Object *object) {
    table_8017b55c[object->field_03](object);
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8017b550 holds this function and is declared with this parameter. */
void func_8014371c(Object *p) {
    func_8011f240((Slab172 *)p);
}

void func_8014373c(Object *object) {
    table_8017b58c[object->field_05](object);
}
