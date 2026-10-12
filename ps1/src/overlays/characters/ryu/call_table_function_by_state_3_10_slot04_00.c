/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801bfec8_slot04_00[];

void call_table_function_by_state_3_10_slot04_00(Object *obj) {
    data_801bfec8_slot04_00[obj->state_3](obj);
}
