/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* The parameter is passed on to reset_and_push_object_to_pool_table, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_8017c978 holds this function and is declared with this parameter. */
void push_object_to_pool_table_wrapper_2(Object *p) {
    reset_and_push_object_to_pool_table((Slab172 *)p);
}

void call_table_function_by_state_0_3(Block172 *block) {
    Object *object = (Object *)block;
    table_8017c9c0[object->state_0](object);
}

void release_object_or_start_sequence_by_index(Object *object) {
    if (object->table_index == 3 || object->table_index == 10) {
        reset_and_push_object_to_pool_table((Slab172 *)object);
    } else {
        object->field_01 = 0;
        object->state_0++;
        set_sequence_from_table_4(object, (SeqRec **)table_8017c988, object->table_index);
    }
}
