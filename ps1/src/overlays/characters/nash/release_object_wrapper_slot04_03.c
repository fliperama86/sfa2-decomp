/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_handler_by_input_dir_0_5_slot04_03(Object *obj);
void call_handler_by_state_3_set_flag_2_slot04_03(Object *obj);
void call_table_function_by_state_3_17_slot04_03(Object *obj);
void call_table_function_by_state_3_18_slot04_03(Object *obj);
extern ObjectFn data_801c09e8_slot04_03[];

void release_object_wrapper_slot04_03(Object *obj, Object *unused) {
    clear_object_and_push_on_free_stack(obj);
}

void call_handler_by_input_dir_0_4_slot04_03(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_5_slot04_03(obj);
    } else {
        call_handler_by_state_3_set_flag_2_slot04_03(obj);
    }
}

void call_handler_by_input_dir_0_5_slot04_03(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_table_function_by_state_3_17_slot04_03(obj);
    } else {
        call_table_function_by_state_3_18_slot04_03(obj);
    }
}

void call_table_function_by_state_3_17_slot04_03(Object *obj) {
    data_801c09e8_slot04_03[obj->state_3](obj);
}
