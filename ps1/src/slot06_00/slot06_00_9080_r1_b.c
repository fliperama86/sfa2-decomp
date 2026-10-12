/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void release_object_slot06_00(Object *object) {
    reset_and_push_object_to_pool_table((Slab172 *)object);
}
