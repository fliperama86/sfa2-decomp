/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ea68_slot28[])(Object *);

void func_80026254_slot28(Object *obj) {
}

void call_handler_by_state_1_18_slot28(Object *object) {
    data_8004ea68_slot28[object->state_1](object);
}
