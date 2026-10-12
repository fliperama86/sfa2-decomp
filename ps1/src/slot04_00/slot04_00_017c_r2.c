/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_2_slot04_00(Object *obj);
void call_handler_by_state_3_set_flag_slot04_00(Object *obj);
void func_801b03bc_slot04_00(Object *obj);
void func_801b05c0_slot04_00(Object *obj);

void call_handler_by_input_dir_0_slot04_00(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_2_slot04_00(obj);
    } else {
        call_handler_by_state_3_set_flag_slot04_00(obj);
    }
}

void call_handler_by_input_dir_0_2_slot04_00(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_801b03bc_slot04_00(obj);
    } else {
        func_801b05c0_slot04_00(obj);
    }
}
