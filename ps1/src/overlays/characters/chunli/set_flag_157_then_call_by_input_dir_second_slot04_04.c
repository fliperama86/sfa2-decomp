/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_state_3_start_move_slot04_04(Object *obj);
void call_table_function_by_state_3_slot04_04(Object *obj);

void set_flag_157_then_call_by_input_dir_second_slot04_04(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_table_function_by_state_3_slot04_04(obj);
    } else {
        update_state_3_start_move_slot04_04(obj);
    }
}
