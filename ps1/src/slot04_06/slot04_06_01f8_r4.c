/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_direction_index_3_slot04_06(Object *obj);
void call_handler_by_direction_index_4_slot04_06(Object *obj);
extern ObjectFn data_801c525c_slot04_06[];
extern ObjectFn data_801c5268_slot04_06[];

void update_sequence_or_finish_sequence_slot04_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void call_direction_handler_by_input_dir_second_slot04_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_4_slot04_06(obj);
    } else {
        call_handler_by_direction_index_3_slot04_06(obj);
    }
}

void call_handler_by_direction_index_3_slot04_06(Object *obj) {
    data_801c525c_slot04_06[obj->direction_index >> 1](obj);
}

void call_table_function_by_state_3_4_slot04_06(Object *obj) {
    data_801c5268_slot04_06[obj->state_3](obj);
}
