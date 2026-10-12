/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3c2c_slot04_08[];

void call_table_function_by_state_3_5_slot04_08(Object *obj) {
    data_801c3c2c_slot04_08[obj->state_3](obj);
}
