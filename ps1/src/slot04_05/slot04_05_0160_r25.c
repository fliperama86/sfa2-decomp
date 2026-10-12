/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void call_handler_by_input_dir_0_2_slot04_05(Object *obj);
void call_handler_by_input_dir_1_slot04_05(Object *obj);
void func_801b3dac_slot04_05(Object *obj);
void func_801b3fe8_slot04_05(Object *obj);
extern ObjectFn data_801c1658_slot04_05[];

void call_handler_by_input_dir_0_slot04_05(Object *obj) {
    if (obj->input_dir_0 == 0) {
        call_handler_by_input_dir_0_2_slot04_05(obj);
    } else {
        call_handler_by_input_dir_1_slot04_05(obj);
    }
}

void call_handler_by_input_dir_0_2_slot04_05(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_801b3dac_slot04_05(obj);
    } else {
        func_801b3fe8_slot04_05(obj);
    }
}

void func_801b3dac_slot04_05(Object *obj) {
    data_801c1658_slot04_05[obj->state_3](obj);
}
