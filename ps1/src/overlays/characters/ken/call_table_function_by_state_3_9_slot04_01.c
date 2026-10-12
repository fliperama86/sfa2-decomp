/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801bf0e8_slot04_01[];

void call_table_function_by_state_3_9_slot04_01(Object *obj) {
    data_801bf0e8_slot04_01[obj->state_3](obj);
}
