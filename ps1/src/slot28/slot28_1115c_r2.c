/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80044608_slot28[])(Object *);
extern ObjectFn data_80044610_slot28[];

void call_table_handler_by_table_index_3_slot28(Object *obj) {
    data_80044608_slot28[obj->table_index](obj);
    obj->field_01 = 1;
}

void func_80021304_slot28(Object *obj) {
    data_80044610_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
}
