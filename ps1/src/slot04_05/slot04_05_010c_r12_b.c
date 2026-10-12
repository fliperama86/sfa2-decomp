/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_table_function_by_state_3_19_slot04_05(Object *object);
void call_table_function_by_state_3_20_slot04_05(Object *object);

void call_handler_by_input_dir_second_2_slot04_05(Object *obj) {
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_20_slot04_05(obj);
    } else {
        call_table_function_by_state_3_19_slot04_05(obj);
    }
}
