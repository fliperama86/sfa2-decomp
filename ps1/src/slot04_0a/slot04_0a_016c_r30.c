/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b45d8_slot04_0a(Object *obj);
void func_801b4828_slot04_0a(Object *obj);
void func_801b4618_slot04_0a(Object *obj);
void func_801b4750_slot04_0a(Object *obj);
extern ObjectFn data_801c0874_slot04_0a[];

void func_801b4598_slot04_0a(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b45d8_slot04_0a(obj);
    } else {
        func_801b4828_slot04_0a(obj);
    }
}

void func_801b45d8_slot04_0a(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b4618_slot04_0a(obj);
    } else {
        func_801b4750_slot04_0a(obj);
    }
}

void func_801b4618_slot04_0a(Object *obj) {
    data_801c0874_slot04_0a[obj->field_07](obj);
}
