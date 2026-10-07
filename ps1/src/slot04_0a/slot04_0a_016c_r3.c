/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b06a0_slot04_0a(Object *obj);
void func_801b085c_slot04_0a(Object *obj);
void func_801b06e0_slot04_0a(Object *obj);
void func_801b07cc_slot04_0a(Object *obj);
extern ObjectFn data_801c0548_slot04_0a[];

void func_801b0660_slot04_0a(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b06a0_slot04_0a(obj);
    } else {
        func_801b085c_slot04_0a(obj);
    }
}

void func_801b06a0_slot04_0a(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b06e0_slot04_0a(obj);
    } else {
        func_801b07cc_slot04_0a(obj);
    }
}

void func_801b06e0_slot04_0a(Object *obj) {
    data_801c0548_slot04_0a[obj->field_07](obj);
}
