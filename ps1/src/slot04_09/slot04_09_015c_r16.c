/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7ce4_slot04_09[];
extern ObjectFn data_801c7cf0_slot04_09[];

void call_handler_by_direction_index_slot04_09(Object *obj) {
    data_801c7ce4_slot04_09[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_9_slot04_09(Object *obj) {
    data_801c7cf0_slot04_09[obj->state_3](obj);
}
