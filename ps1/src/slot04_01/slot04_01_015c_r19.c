/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b404c_slot04_01(Object *obj);
void func_801b4410_slot04_01(Object *obj);
void func_801b408c_slot04_01(Object *obj);
void func_801b41c4_slot04_01(Object *obj);
extern ObjectFn data_801bf280_slot04_01[];

void func_801b400c_slot04_01(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b404c_slot04_01(obj);
    } else {
        func_801b4410_slot04_01(obj);
    }
}

void func_801b404c_slot04_01(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b408c_slot04_01(obj);
    } else {
        func_801b41c4_slot04_01(obj);
    }
}

void func_801b408c_slot04_01(Object *obj) {
    data_801bf280_slot04_01[obj->field_07](obj);
}
