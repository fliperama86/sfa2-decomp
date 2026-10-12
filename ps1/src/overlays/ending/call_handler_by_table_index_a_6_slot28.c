/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn data_800422e4_slot28[];
extern void (*data_80042308_slot28[])(Object *);

void call_handler_by_table_index_a_6_slot28(Object *obj) {
    data_800422e4_slot28[obj->table_index](obj);
}

void call_state_1_handler_c_advance_and_show_2_slot28(Object *obj) {
    data_80042308_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}
