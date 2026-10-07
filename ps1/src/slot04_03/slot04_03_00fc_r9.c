/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c05f8_slot04_03[];
extern ObjectFn data_801c0604_slot04_03[];
extern ObjectFn data_801c061c_slot04_03[];
extern ObjectFn data_801c0638_slot04_03[];

void func_801b1834_slot04_03(Object *obj) {
    data_801c05f8_slot04_03[obj->field_12a >> 1](obj);
}

void func_801b1878_slot04_03(Object *obj) {
    data_801c0604_slot04_03[obj->field_07](obj);
}

void func_801b18b8_slot04_03(Object *obj) {
    data_801c061c_slot04_03[obj->field_07](obj);
}

void func_801b18f8_slot04_03(Object *obj) {
    data_801c0638_slot04_03[obj->field_07](obj);
}
