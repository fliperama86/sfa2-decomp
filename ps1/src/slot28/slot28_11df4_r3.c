/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800469e4_slot28[])(Object *);

void func_800220d4_slot28(Object *obj) {
}

void call_state_1_handler_c_advance_and_show_3_slot28(Object *obj) {
    data_800469e4_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}
