/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_2_slot04_03(Object *obj);
void call_handler_by_state_3_set_flag_slot04_03(Object *obj);
void call_state_3_handler_then_noop_slot04_03(Object *obj);
void call_state_3_handler_then_noop_2_slot04_03(Object *obj);

void call_handler_by_input_dir_0_slot04_03(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_2_slot04_03(obj);
    } else {
        call_handler_by_state_3_set_flag_slot04_03(obj);
    }
}

void call_handler_by_input_dir_0_2_slot04_03(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_state_3_handler_then_noop_slot04_03(obj);
    } else {
        call_state_3_handler_then_noop_2_slot04_03(obj);
    }
}
