/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c24e8_slot04_0b[];

void load_velocities_from_table_2_slot04_0b(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_table_function_by_state_3_5_slot04_0b(Object *obj) {
    data_801c24e8_slot04_0b[obj->state_3](obj);
}
