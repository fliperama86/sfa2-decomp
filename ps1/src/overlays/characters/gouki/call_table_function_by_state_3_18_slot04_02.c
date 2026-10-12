/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c6420_slot04_02[];
extern ObjectFn data_801c646c_slot04_02[];

void call_table_function_by_state_3_18_slot04_02(Object *obj) {
    data_801c6420_slot04_02[obj->state_3](obj);
}

void call_table_function_by_state_3_19_slot04_02(Object *obj) {
    data_801c646c_slot04_02[obj->state_3](obj);
}
