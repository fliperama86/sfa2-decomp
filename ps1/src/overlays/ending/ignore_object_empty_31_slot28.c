/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8004ab1c_slot28[])(Object *);

void ignore_object_empty_31_slot28(Object *obj) {
}

void call_handler_by_state_1_14_slot28(Object *object) {
    data_8004ab1c_slot28[object->state_1](object);
}
