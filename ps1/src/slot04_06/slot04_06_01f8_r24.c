/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_second_2_slot04_06(Object *obj);
void func_801b56dc_slot04_06(Object *obj);
void call_handler_by_direction_index_5_slot04_06(Object *obj);
void call_handler_by_direction_index_6_slot04_06(Object *obj);
extern ObjectFn data_801c5550_slot04_06[];
extern ObjectFn data_801c555c_slot04_06[];

void call_handler_by_input_dir_0_nonzero_2_slot04_06(Object *obj) {
    if (obj->input_dir_0 != 0) {
        func_801b56dc_slot04_06(obj);
    } else {
        call_handler_by_input_dir_second_2_slot04_06(obj);
    }
}

void call_handler_by_input_dir_second_2_slot04_06(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        call_handler_by_direction_index_6_slot04_06(obj);
    } else {
        call_handler_by_direction_index_5_slot04_06(obj);
    }
}

void call_handler_by_direction_index_5_slot04_06(Object *obj) {
    data_801c5550_slot04_06[obj->direction_index >> 1](obj);
}

void func_801b52e0_slot04_06(Object *obj) {
    data_801c555c_slot04_06[obj->state_3](obj);
}
