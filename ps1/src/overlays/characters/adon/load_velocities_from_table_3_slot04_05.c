/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c1390_slot04_05[];

void load_velocities_from_table_3_slot04_05(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_table_function_by_state_3_5_slot04_05(Object *obj) {
    data_801c1390_slot04_05[obj->state_3](obj);
}
