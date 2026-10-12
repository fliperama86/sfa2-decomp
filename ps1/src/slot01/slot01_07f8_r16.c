/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015dc0_slot01[];

void load_velocities_from_table_3_slot01(Object *obj) {
    func_8011ffdc(obj);
}

void release_object_record_8_slot01(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_5_slot01(Object *object) {
    data_80015dc0_slot01[object->state_0](object);
}
