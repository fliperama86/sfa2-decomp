/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c1574_slot04_05[];
extern ObjectFn data_801c1580_slot04_05[];

void call_handler_by_direction_index_2_slot04_05(Object *obj) {
    data_801c1574_slot04_05[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_13_slot04_05(Object *obj) {
    data_801c1580_slot04_05[obj->state_3](obj);
}
