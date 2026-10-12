/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003a184_slot28[])(Object *);

void func_8001c81c_slot28(Object *obj) {
}

void call_handler_by_state_1_8_slot28(Object *object) {
    data_8003a184_slot28[object->state_1](object);
}
