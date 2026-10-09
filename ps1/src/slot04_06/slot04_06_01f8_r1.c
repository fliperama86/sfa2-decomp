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
    if (obj->field_128 != 0) {
        func_801b0630_slot04_06(obj);
    } else {
        func_801b0238_slot04_06(obj);
    }
}

void func_801b0238_slot04_06(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b03a4_slot04_06(obj);
    } else {
        func_801b0278_slot04_06(obj);
    }
}

void func_801b0278_slot04_06(Object *obj) {
    data_801c5228_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b02bc_slot04_06(Object *obj) {
    data_801c5234_slot04_06[obj->field_07](obj);
}
