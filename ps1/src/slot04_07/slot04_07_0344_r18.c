/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_4_slot04_07(Object *obj);
void call_handler_by_input_dir_1_slot04_07(Object *obj);
void func_801b4444_slot04_07(Object *obj);
void func_801b46cc_slot04_07(Object *obj);

void call_handler_by_input_dir_0_3_slot04_07(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_4_slot04_07(obj);
    } else {
        call_handler_by_input_dir_1_slot04_07(obj);
    }
}

void call_handler_by_input_dir_0_4_slot04_07(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_801b4444_slot04_07(obj);
    } else {
        func_801b46cc_slot04_07(obj);
    }
}
