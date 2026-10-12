/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_table_function_by_state_3_12_slot04_04(Object *obj);
void call_table_function_by_state_3_11_slot04_04(Object *obj);

void call_state_3_dispatch_by_input_dir_0_slot04_04(Object *obj) {
    if (obj->input_dir_0 == 4) {
        call_table_function_by_state_3_12_slot04_04(obj);
    } else {
        call_table_function_by_state_3_11_slot04_04(obj);
    }
}
