/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ab38_slot28[])(Object *);

void ignore_object_empty_33_slot28(Object *obj) {
}

void call_handler_by_state_1_16_slot28(Object *object) {
    data_8004ab38_slot28[object->state_1](object);
}
