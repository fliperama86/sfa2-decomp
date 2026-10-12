/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c523c_slot04_06[];
extern ObjectFn data_801c5248_slot04_06[];

void load_velocities_from_table_slot04_06(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_handler_by_direction_index_2_slot04_06(Object *obj) {
    data_801c523c_slot04_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_2_slot04_06(Object *obj) {
    data_801c5248_slot04_06[obj->state_3](obj);
}
