/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0614_slot04_0a[];
extern ObjectFn data_801c0634_slot04_0a[];

void call_table_function_by_state_3_5_slot04_0a(Object *obj) {
    data_801c0614_slot04_0a[obj->state_3](obj);
}

void call_table_function_by_state_3_6_slot04_0a(Object *obj) {
    data_801c0634_slot04_0a[obj->state_3](obj);
}
