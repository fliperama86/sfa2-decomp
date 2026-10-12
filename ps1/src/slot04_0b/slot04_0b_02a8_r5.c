/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b07cc_slot04_0b(Object *obj);
void func_801b0ad4_slot04_0b(Object *obj);
void func_801b080c_slot04_0b(Object *obj);
void func_801b08f4_slot04_0b(Object *obj);
extern ObjectFn data_801c24cc_slot04_0b[];

void func_801b078c_slot04_0b(Object *obj) {
    if (obj->input_dir_0 == 0) {
        func_801b07cc_slot04_0b(obj);
    } else {
        func_801b0ad4_slot04_0b(obj);
    }
}

void func_801b07cc_slot04_0b(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second == 0) {
        func_801b080c_slot04_0b(obj);
    } else {
        func_801b08f4_slot04_0b(obj);
    }
}

void func_801b080c_slot04_0b(Object *obj) {
    data_801c24cc_slot04_0b[obj->state_3](obj);
}
