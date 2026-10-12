/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80037488_slot28[])(Object *);

void set_object_flag_byte_to_one_2_slot28(Object *obj) {
    obj->field_01 = 1;
}

void call_state_1_handler_d_and_show_4_slot28(Object *obj) {
    data_80037488_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
