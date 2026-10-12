/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_4_slot04_0a(Object *obj);
void call_handler_by_input_dir_1_slot04_0a(Object *obj);
void call_table_function_by_state_3_20_slot04_0a(Object *obj);
void call_table_function_by_state_3_21_slot04_0a(Object *obj);
extern ObjectFn data_801c0874_slot04_0a[];

void call_handler_by_input_dir_0_3_slot04_0a(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_4_slot04_0a(obj);
    } else {
        call_handler_by_input_dir_1_slot04_0a(obj);
    }
}

void call_handler_by_input_dir_0_4_slot04_0a(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_table_function_by_state_3_20_slot04_0a(obj);
    } else {
        call_table_function_by_state_3_21_slot04_0a(obj);
    }
}

void call_table_function_by_state_3_20_slot04_0a(Object *obj) {
    data_801c0874_slot04_0a[obj->state_3](obj);
}
