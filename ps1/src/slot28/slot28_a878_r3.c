/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80037448_slot28[];
extern void (*data_8003746c_slot28[])(Object *);

void call_handler_by_table_index_a_3_slot28(Object *obj) {
    data_80037448_slot28[obj->table_index](obj);
}

void func_8001abfc_slot28(Object *obj) {
    data_8003746c_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
