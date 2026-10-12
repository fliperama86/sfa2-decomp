/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_2_slot04_01(Object *obj);
void start_or_continue_move_by_state_3_slot04_01(Object *obj);
void update_state_3_check_range_or_continue_slot04_01(Object *obj);
void func_801b0458_slot04_01(Object *obj);

void call_handler_by_input_dir_0_slot04_01(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_2_slot04_01(obj);
    } else {
        start_or_continue_move_by_state_3_slot04_01(obj);
    }
}

void call_handler_by_input_dir_0_2_slot04_01(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        update_state_3_check_range_or_continue_slot04_01(obj);
    } else {
        func_801b0458_slot04_01(obj);
    }
}
