/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c523c_slot04_06[];
extern ObjectFn data_801c5248_slot04_06[];

void func_801b0384_slot04_06(Object *obj) {
    func_80142a14(obj);
}

void func_801b03a4_slot04_06(Object *obj) {
    data_801c523c_slot04_06[obj->field_12a >> 1](obj);
}

void func_801b03e8_slot04_06(Object *obj) {
    data_801c5248_slot04_06[obj->field_07](obj);
}
