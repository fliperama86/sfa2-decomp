/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80035414_slot28[])(Object *);

void advance_state_0_on_variant_index_set_2_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_0++;
    }
}

void call_handler_by_state_1_6_slot28(Object *object) {
    data_80035414_slot28[object->state_1](object);
}
