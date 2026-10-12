/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_handler_by_input_dir_0_2_slot04_07(Object *obj);
void call_handler_by_input_dir_second_zero_slot04_07(Object *obj);
void call_state_3_handler_then_play_step_sound_slot04_07(Object *obj);
void call_table_function_by_state_3_slot04_07(Object *obj);

void call_handler_by_input_dir_0_slot04_07(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_2_slot04_07(obj);
    } else {
        call_handler_by_input_dir_second_zero_slot04_07(obj);
    }
}

void call_handler_by_input_dir_0_2_slot04_07(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_state_3_handler_then_play_step_sound_slot04_07(obj);
    } else {
        call_table_function_by_state_3_slot04_07(obj);
    }
}
