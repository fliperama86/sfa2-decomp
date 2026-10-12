/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8003f9d4_slot28[];
extern void (*data_8003f9e0_slot28[])(Object *);

void call_handler_by_table_index_a_5_slot28(Object *obj) {
    data_8003f9d4_slot28[obj->table_index](obj);
}

void func_8001f0d4_slot28(Object *obj) {
    data_8003f9e0_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
