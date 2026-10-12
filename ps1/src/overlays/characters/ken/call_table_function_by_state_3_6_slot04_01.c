/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801bf0c8_slot04_01[];

void call_table_function_by_state_3_6_slot04_01(Object *obj) {
    data_801bf0c8_slot04_01[obj->state_3](obj);
}
