/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c14a8_slot04_05[];

void call_table_function_by_state_3_8_slot04_05(Object *obj) {
    data_801c14a8_slot04_05[obj->state_3](obj);
}
