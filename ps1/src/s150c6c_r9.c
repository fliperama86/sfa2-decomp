/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8018023c holds this function and is declared with this parameter. */
void push_object_to_pool_table_wrapper_10(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

void call_table_function_by_state_0_11(Block172 *block) {
    Object *object = (Object *)block;
    table_80180274[object->state_0](object);
}
