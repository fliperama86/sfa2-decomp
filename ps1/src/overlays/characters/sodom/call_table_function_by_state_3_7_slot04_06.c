/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c52e8_slot04_06[];

void call_table_function_by_state_3_7_slot04_06(Object *obj) {
    data_801c52e8_slot04_06[obj->state_3](obj);
}
