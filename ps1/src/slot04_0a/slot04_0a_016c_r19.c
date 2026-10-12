/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c06e4_slot04_0a[];

void call_table_function_by_state_3_14_slot04_0a(Object *obj) {
    data_801c06e4_slot04_0a[obj->state_3](obj);
}
