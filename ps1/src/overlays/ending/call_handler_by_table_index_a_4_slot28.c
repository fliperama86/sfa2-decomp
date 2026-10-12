/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn data_8003a160_slot28[];
extern void (*data_8003a174_slot28[])(Object *);

void call_handler_by_table_index_a_4_slot28(Object *obj) {
    data_8003a160_slot28[obj->table_index](obj);
}

void call_state_1_handler_d_and_show_6_slot28(Object *obj) {
    data_8003a174_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
