/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c3cdc_slot04_08[];
extern ObjectFn data_801c3ce8_slot04_08[];
extern ObjectFn data_801c3d04_slot04_08[];
extern ObjectFn data_801c3d20_slot04_08[];

void call_handler_by_direction_index_slot04_08(Object *obj) {
    data_801c3cdc_slot04_08[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_7_slot04_08(Object *obj) {
    data_801c3ce8_slot04_08[obj->state_3](obj);
}

void call_table_function_by_state_3_8_slot04_08(Object *obj) {
    data_801c3d04_slot04_08[obj->state_3](obj);
}

void call_table_function_by_state_3_9_slot04_08(Object *obj) {
    data_801c3d20_slot04_08[obj->state_3](obj);
}
