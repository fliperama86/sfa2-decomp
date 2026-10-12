/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bfdf0_slot04_00[];

void call_table_function_by_state_3_slot04_00(Object *obj) {
    data_801bfdf0_slot04_00[obj->state_3](obj);
}
