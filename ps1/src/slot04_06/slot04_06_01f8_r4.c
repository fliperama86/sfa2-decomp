/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0670_slot04_06(Object *obj);
void func_801b0744_slot04_06(Object *obj);
extern ObjectFn data_801c525c_slot04_06[];
extern ObjectFn data_801c5268_slot04_06[];

void func_801b05f0_slot04_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b0630_slot04_06(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b0744_slot04_06(obj);
    } else {
        func_801b0670_slot04_06(obj);
    }
}

void func_801b0670_slot04_06(Object *obj) {
    data_801c525c_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b06b4_slot04_06(Object *obj) {
    data_801c5268_slot04_06[obj->field_07](obj);
}
