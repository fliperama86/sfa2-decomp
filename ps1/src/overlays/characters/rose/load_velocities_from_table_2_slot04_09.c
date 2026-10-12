/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_table_function_by_state_3_3_slot04_09(Object *obj);
void call_table_function_by_state_3_4_slot04_09(Object *obj);
extern ObjectFn data_801c7bc4_slot04_09[];

void load_velocities_from_table_2_slot04_09(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_state_3_dispatch_by_input_dir_second_a_slot04_09(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_4_slot04_09(obj);
    } else {
        call_table_function_by_state_3_3_slot04_09(obj);
    }
}

void call_table_function_by_state_3_3_slot04_09(Object *obj) {
    data_801c7bc4_slot04_09[obj->state_3](obj);
}
