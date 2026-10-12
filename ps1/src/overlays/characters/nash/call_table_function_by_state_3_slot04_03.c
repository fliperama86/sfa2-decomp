/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c05d4_slot04_03[];

void call_table_function_by_state_3_slot04_03(Object *obj) {
    data_801c05d4_slot04_03[obj->state_3](obj);
}
