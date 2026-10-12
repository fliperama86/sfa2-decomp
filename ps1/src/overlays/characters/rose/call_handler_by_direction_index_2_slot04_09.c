/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c7d74_slot04_09[];
extern ObjectFn data_801c7d80_slot04_09[];

void call_handler_by_direction_index_2_slot04_09(Object *obj) {
    data_801c7d74_slot04_09[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_12_slot04_09(Object *obj) {
    data_801c7d80_slot04_09[obj->state_3](obj);
}
