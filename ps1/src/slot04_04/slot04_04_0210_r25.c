/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b432c_slot04_04(Object *obj);
void func_801b445c_slot04_04(Object *obj);

void call_handler_by_input_dir_second_2_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b445c_slot04_04(obj);
    } else {
        func_801b432c_slot04_04(obj);
    }
}
