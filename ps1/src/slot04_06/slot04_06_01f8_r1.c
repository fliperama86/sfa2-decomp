/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0238_slot04_06(Object *obj);
void func_801b0630_slot04_06(Object *obj);
void func_801b0278_slot04_06(Object *obj);
void func_801b03a4_slot04_06(Object *obj);
extern ObjectFn data_801c5228_slot04_06[];
extern ObjectFn data_801c5234_slot04_06[];

void func_801b01f8_slot04_06(Object *obj) {
    if (obj->input_dir_0 != 0) {
        func_801b0630_slot04_06(obj);
    } else {
        func_801b0238_slot04_06(obj);
    }
}

void func_801b0238_slot04_06(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b03a4_slot04_06(obj);
    } else {
        func_801b0278_slot04_06(obj);
    }
}

void func_801b0278_slot04_06(Object *obj) {
    data_801c5228_slot04_06[obj->direction_index >> 1](obj);
}

void func_801b02bc_slot04_06(Object *obj) {
    data_801c5234_slot04_06[obj->state_3](obj);
}
