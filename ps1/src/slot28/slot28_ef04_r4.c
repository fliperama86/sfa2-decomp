/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003f9f0_slot28[])(Object *);

void ignore_object_empty_24_slot28(Object *obj) {
}

void call_state_1_handler_d_and_show_9_slot28(Object *obj) {
    data_8003f9f0_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
