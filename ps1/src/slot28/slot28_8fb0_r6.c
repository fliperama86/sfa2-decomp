/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800353d4_slot28[])(Object *);

void func_800194c8_slot28(Object *obj) {
}

void call_handler_by_state_1_4_slot28(Object *object) {
    data_800353d4_slot28[object->state_1](object);
}
