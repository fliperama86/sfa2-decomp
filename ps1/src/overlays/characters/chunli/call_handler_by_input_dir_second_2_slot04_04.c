/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_by_state_3_zero_check_range_or_update_slot04_04(Object *obj);
void call_handler_by_state_3_slot04_04(Object *obj);

void call_handler_by_input_dir_second_2_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_state_3_slot04_04(obj);
    } else {
        call_by_state_3_zero_check_range_or_update_slot04_04(obj);
    }
}
