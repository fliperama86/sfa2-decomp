/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c20b0_slot04_07[];

void call_table_function_by_state_3_9_slot04_07(Object *obj) {
    data_801c20b0_slot04_07[obj->state_3](obj);
}
