/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void release_object_record_3_slot04_06(Object *obj);

void load_velocities_from_table_7_slot04_06(Object *obj) {
    release_object_record_3_slot04_06(obj);
}

void release_object_record_3_slot04_06(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
