/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c5598_slot04_06[];
extern ObjectFn data_801c55a4_slot04_06[];

void call_handler_by_direction_index_8_slot04_06(Object *obj) {
    data_801c5598_slot04_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_24_slot04_06(Object *obj) {
    data_801c55a4_slot04_06[obj->state_3](obj);
}

void advance_state_3_restore_facing_set_sequence_2_slot04_06(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    set_sequence_and_tables_from_direction(obj);
}
