/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80148e44(Object *object) {
    reset_and_push_object_to_pool_table(object);
}

void func_80148e64(Object *object) {
    reset_and_push_object_to_pool_table(object);
}

u8 func_80148e84(Object *object) {
    return spawn_random_offset_object_with_variant(object, 0);
}

u8 func_80148ea8(Object *object) {
    return spawn_random_offset_object_with_variant(object, 1);
}
