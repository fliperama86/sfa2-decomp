/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_handler_by_table_index(Object *object) {
    table_8017b55c[object->table_index](object);
}

/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8017b550 holds this function and is declared with this parameter. */
void push_object_to_pool_table_wrapper(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

void call_state_1_handler(Object *object) {
    table_8017b58c[object->state_1](object);
}
