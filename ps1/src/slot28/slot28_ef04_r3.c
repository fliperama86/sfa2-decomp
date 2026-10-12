/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003f9e8_slot28[])(Object *);

void ignore_object_empty_23_slot28(Object *obj) {
}

void call_state_1_handler_d_and_show_8_slot28(Object *obj) {
    data_8003f9e8_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
