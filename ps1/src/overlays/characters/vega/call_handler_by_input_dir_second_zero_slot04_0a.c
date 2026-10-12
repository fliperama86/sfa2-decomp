/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_table_function_by_state_3_3_slot04_0a(Object *obj);
void call_handler_by_state_3_or_alt_table_slot04_0a(Object *obj);

void call_handler_by_input_dir_second_zero_slot04_0a(Object *obj) {
    if (obj->input_dir_second == 0) {
        call_table_function_by_state_3_3_slot04_0a(obj);
    } else {
        call_handler_by_state_3_or_alt_table_slot04_0a(obj);
    }
}
