/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b525c_slot04_06(Object *obj);
void func_801b56dc_slot04_06(Object *obj);
void func_801b529c_slot04_06(Object *obj);
void func_801b5414_slot04_06(Object *obj);
extern ObjectFn data_801c5550_slot04_06[];
extern ObjectFn data_801c555c_slot04_06[];

void func_801b521c_slot04_06(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b56dc_slot04_06(obj);
    } else {
        func_801b525c_slot04_06(obj);
    }
}

void func_801b525c_slot04_06(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b5414_slot04_06(obj);
    } else {
        func_801b529c_slot04_06(obj);
    }
}

void func_801b529c_slot04_06(Object *obj) {
    data_801c5550_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b52e0_slot04_06(Object *obj) {
    data_801c555c_slot04_06[obj->field_07](obj);
}
