/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079804_slot2b[];
extern ObjectFn data_80079810_slot2b[];
void call_handler_by_input_dir_second_slot2b(Object *obj);
void call_handler_by_direction_index_slot2b(Object *obj);
void call_handler_by_direction_index_2_slot2b(Object *obj);
void call_direction_handler_by_input_dir_second_slot2b(Object *obj);

void call_handler_by_input_dir_0_nonzero_slot2b(Object *obj) {
    if (obj->input_dir_0 != 0) {
        call_direction_handler_by_input_dir_second_slot2b(obj);
    } else {
        call_handler_by_input_dir_second_slot2b(obj);
    }
}

void call_handler_by_input_dir_second_slot2b(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_2_slot2b(obj);
    } else {
        call_handler_by_direction_index_slot2b(obj);
    }
}

void call_handler_by_direction_index_slot2b(Object *obj) {
    data_80079804_slot2b[obj->direction_index >> 1](obj);
}

void call_handler_by_state_3_slot2b(Object *obj) {
    data_80079810_slot2b[obj->state_3](obj);
}
