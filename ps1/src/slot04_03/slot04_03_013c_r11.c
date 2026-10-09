/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c058c_slot04_03[];
extern ObjectFn data_801c05b0_slot04_03[];

int func_801b1238_slot04_03(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b1244_slot04_03(Object *obj) {
    data_801c058c_slot04_03[obj->field_15a](obj);
}

void func_801b1284_slot04_03(Object *obj) {
    data_801c05b0_slot04_03[obj->field_15a](obj);
}
