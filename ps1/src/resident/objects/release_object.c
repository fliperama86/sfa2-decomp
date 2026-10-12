/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void release_object(Object *object) {
    reset_and_push_object_to_pool_table((Slab172 *)object);
}

void release_object_2(Object *object) {
    reset_and_push_object_to_pool_table((Slab172 *)object);
}
