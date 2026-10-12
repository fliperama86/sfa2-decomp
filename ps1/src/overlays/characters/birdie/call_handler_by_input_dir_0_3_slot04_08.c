/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_handler_by_input_dir_0_4_slot04_08(Object *obj);
void call_handler_by_state_3_with_flag_slot04_08(Object *obj);
void call_table_function_by_state_3_16_slot04_08(Object *obj);
void call_table_function_by_state_3_17_slot04_08(Object *obj);
extern ObjectFn data_801c3de4_slot04_08[];

void call_handler_by_input_dir_0_3_slot04_08(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_4_slot04_08(obj);
    } else {
        call_handler_by_state_3_with_flag_slot04_08(obj);
    }
}

void call_handler_by_input_dir_0_4_slot04_08(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_table_function_by_state_3_16_slot04_08(obj);
    } else {
        call_table_function_by_state_3_17_slot04_08(obj);
    }
}

void call_table_function_by_state_3_16_slot04_08(Object *obj) {
    data_801c3de4_slot04_08[obj->state_3](obj);
}
