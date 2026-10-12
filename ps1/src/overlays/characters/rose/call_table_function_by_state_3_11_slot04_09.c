/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c7d38_slot04_09[];

void call_table_function_by_state_3_11_slot04_09(Object *obj) {
    data_801c7d38_slot04_09[obj->state_3](obj);
}
