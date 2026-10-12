/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn data_80015a50_slot01[];

void release_object_record_5_slot01(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_4_slot01(Object *object) {
    data_80015a50_slot01[object->state_0](object);
}
