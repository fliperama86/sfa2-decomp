/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_table_function_by_state_3_21_slot04_02(Object *object);
void call_table_function_by_state_3_22_slot04_02(Object *object);

void call_handler_by_direction_input_slot04_02(Object *obj) {
    if (obj->input_dir_second == 0 || obj->direction_index != 4) {
        call_table_function_by_state_3_21_slot04_02(obj);
    } else {
        call_table_function_by_state_3_22_slot04_02(obj);
    }
}
