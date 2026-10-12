/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_table_function_by_state_3_18_slot04_0a(Object *obj);
void call_table_function_by_state_3_19_slot04_0a(Object *obj);
extern ObjectFn data_801c0714_slot04_0a[];

void call_handler_by_input_dir_0_nonzero_slot04_0a(Object *obj) {
    if (obj->input_dir_0 != 0) {
        call_table_function_by_state_3_19_slot04_0a(obj);
    } else {
        call_table_function_by_state_3_18_slot04_0a(obj);
    }
}

void call_table_function_by_state_3_18_slot04_0a(Object *obj) {
    data_801c0714_slot04_0a[obj->state_3](obj);
}
