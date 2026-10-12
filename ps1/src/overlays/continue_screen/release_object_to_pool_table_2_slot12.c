/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn table_80022e14_slot12[];

void release_object_to_pool_table_2_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_2_slot12(Object *obj) {
    table_80022e14_slot12[obj->state_0](obj);
}
