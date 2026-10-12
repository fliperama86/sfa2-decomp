/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_table_function_by_state_3_23_slot04_02(Object *obj);
void call_table_function_by_state_3_24_slot04_02(Object *obj);
extern ObjectFn data_801c65a0_slot04_02[];

void call_handler_by_input_dir_second_2_slot04_02(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_24_slot04_02(obj);
    } else {
        call_table_function_by_state_3_23_slot04_02(obj);
    }
}

void call_table_function_by_state_3_23_slot04_02(Object *obj) {
    data_801c65a0_slot04_02[obj->state_3](obj);
}
