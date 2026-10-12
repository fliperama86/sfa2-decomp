/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c2664_slot04_0b[];

void call_table_function_by_state_3_14_slot04_0b(Object *obj) {
    data_801c2664_slot04_0b[obj->state_3](obj);
}
