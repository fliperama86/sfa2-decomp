/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5270_slot04_06[];
extern ObjectFn data_801c527c_slot04_06[];

void func_801b0724_slot04_06(Object *obj) {
    func_80142a14(obj);
}

void func_801b0744_slot04_06(Object *obj) {
    data_801c5270_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b0788_slot04_06(Object *obj) {
    data_801c527c_slot04_06[obj->field_07](obj);
}
