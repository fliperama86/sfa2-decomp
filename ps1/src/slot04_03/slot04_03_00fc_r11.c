/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c06a8_slot04_03[];
extern ObjectFn data_801c06d8_slot04_03[];

void call_table_function_by_state_3_7_slot04_03(Object *obj) {
    data_801c06a8_slot04_03[obj->state_3](obj);
}

void call_table_function_by_state_3_8_slot04_03(Object *obj) {
    data_801c06d8_slot04_03[obj->state_3](obj);
}
