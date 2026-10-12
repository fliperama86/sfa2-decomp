/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_2_slot04_0b(Object *obj);
void call_handler_by_state_3_set_flag_slot04_0b(Object *obj);
void func_801b080c_slot04_0b(Object *obj);
void call_handler_by_direction_index_slot04_0b(Object *obj);
extern ObjectFn data_801c24cc_slot04_0b[];

void call_handler_by_input_dir_0_slot04_0b(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_2_slot04_0b(obj);
    } else {
        call_handler_by_state_3_set_flag_slot04_0b(obj);
    }
}

void call_handler_by_input_dir_0_2_slot04_0b(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_801b080c_slot04_0b(obj);
    } else {
        call_handler_by_direction_index_slot04_0b(obj);
    }
}

void func_801b080c_slot04_0b(Object *obj) {
    data_801c24cc_slot04_0b[obj->state_3](obj);
}
