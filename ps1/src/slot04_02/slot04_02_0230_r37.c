/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void increment_state_0_2_slot04_02(Object *o) {
    o->state_0++;
}

void release_object_record_slot04_02(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
