/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c42a4_slot04_04[];

void call_table_function_by_state_3_3_slot04_04(Object *obj) {
    data_801c42a4_slot04_04[obj->state_3](obj);
}
