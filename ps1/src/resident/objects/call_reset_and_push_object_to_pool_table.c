/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_reset_and_push_object_to_pool_table(Object *object) {
    reset_and_push_object_to_pool_table(object);
}

void call_reset_and_push_object_to_pool_table_2(Object *object) {
    reset_and_push_object_to_pool_table(object);
}

u8 spawn_random_offset_object_variant_zero(Object *object) {
    return spawn_random_offset_object_with_variant(object, 0);
}

u8 spawn_random_offset_object_variant_one(Object *object) {
    return spawn_random_offset_object_with_variant(object, 1);
}
