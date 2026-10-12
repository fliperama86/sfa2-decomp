/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_second_slot04_05(Object *obj);
void call_state_3_dispatch_by_input_dir_second_a_slot04_05(Object *obj);
void call_table_function_by_state_3_2_slot04_05(Object *obj);
void call_table_function_by_state_3_3_slot04_05(Object *obj);
extern ObjectFn data_801c1374_slot04_05[];

void call_handler_by_input_dir_0_nonzero_slot04_05(Object *obj) {
    if (obj->input_dir_0 != 0) {
        call_state_3_dispatch_by_input_dir_second_a_slot04_05(obj);
    } else {
        call_handler_by_input_dir_second_slot04_05(obj);
    }
}

void call_handler_by_input_dir_second_slot04_05(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_3_slot04_05(obj);
    } else {
        call_table_function_by_state_3_2_slot04_05(obj);
    }
}

void call_table_function_by_state_3_2_slot04_05(Object *obj) {
    data_801c1374_slot04_05[obj->state_3](obj);
}
