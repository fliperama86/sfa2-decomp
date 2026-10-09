/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to func_8011f240, which takes it: the original sets no argument register before that call, so the callee receives what this function's caller passed. No unit of the tree calls this function by name. */
void func_80144ec4(Slab172 *p) {
    func_8011f240(p);
}

void func_80144ee4(Object *object, s16 index) {
    func_80130768(object, index, (SequenceStep **)table_8017b50c);
}
