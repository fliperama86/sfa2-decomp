/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_handler_by_input_dir_0_4_slot04_01(Object *obj);
void call_handler_by_state_3_set_flag_slot04_01(Object *obj);
void call_table_function_by_state_3_11_slot04_01(Object *obj);
void call_state_3_table_then_play_step_flags_sound_6_slot04_01(Object *obj);
extern ObjectFn data_801bf280_slot04_01[];

void call_handler_by_input_dir_0_3_slot04_01(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_4_slot04_01(obj);
    } else {
        call_handler_by_state_3_set_flag_slot04_01(obj);
    }
}

void call_handler_by_input_dir_0_4_slot04_01(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        call_table_function_by_state_3_11_slot04_01(obj);
    } else {
        call_state_3_table_then_play_step_flags_sound_6_slot04_01(obj);
    }
}

void call_table_function_by_state_3_11_slot04_01(Object *obj) {
    data_801bf280_slot04_01[obj->state_3](obj);
}
