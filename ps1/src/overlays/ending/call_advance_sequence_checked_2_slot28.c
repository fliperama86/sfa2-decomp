/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void call_advance_sequence_checked_2_slot28(Object *obj) {
    advance_sequence_checked(obj);
}

void release_object_to_pool_table_16_slot28(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
