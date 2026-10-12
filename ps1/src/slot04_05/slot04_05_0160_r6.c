/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_table_function_by_state_3_4_slot04_05(Object *obj);
void call_table_function_by_state_3_5_slot04_05(Object *obj);
extern ObjectFn data_801c1388_slot04_05[];

void load_velocities_from_table_2_slot04_05(Object *obj) {
    update_sequence_with_input_check(obj);
}

void call_state_3_dispatch_by_input_dir_second_a_slot04_05(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_5_slot04_05(obj);
    } else {
        call_table_function_by_state_3_4_slot04_05(obj);
    }
}

void call_table_function_by_state_3_4_slot04_05(Object *obj) {
    data_801c1388_slot04_05[obj->state_3](obj);
}
