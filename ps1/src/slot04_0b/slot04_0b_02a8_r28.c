/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_4_slot04_0b(Object *obj);
void call_handler_by_state_3_set_flag_2_slot04_0b(Object *obj);
void func_801b4174_slot04_0b(Object *obj);
void func_801b42ac_slot04_0b(Object *obj);
extern ObjectFn data_801c2f3c_slot04_0b[];

void call_handler_by_input_dir_0_3_slot04_0b(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_4_slot04_0b(obj);
    } else {
        call_handler_by_state_3_set_flag_2_slot04_0b(obj);
    }
}

void call_handler_by_input_dir_0_4_slot04_0b(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_801b4174_slot04_0b(obj);
    } else {
        func_801b42ac_slot04_0b(obj);
    }
}

void func_801b4174_slot04_0b(Object *obj) {
    data_801c2f3c_slot04_0b[obj->state_3](obj);
}
