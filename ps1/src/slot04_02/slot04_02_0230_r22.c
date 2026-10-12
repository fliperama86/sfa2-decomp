/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6538_slot04_02[];

void call_table_function_by_state_3_21_slot04_02(Object *obj) {
    data_801c6538_slot04_02[obj->state_3](obj);
}
