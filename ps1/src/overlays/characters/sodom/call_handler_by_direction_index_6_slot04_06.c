/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c5564_slot04_06[];
extern ObjectFn data_801c5570_slot04_06[];

void call_handler_by_direction_index_6_slot04_06(Object *obj) {
    data_801c5564_slot04_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_21_slot04_06(Object *obj) {
    data_801c5570_slot04_06[obj->state_3](obj);
}
