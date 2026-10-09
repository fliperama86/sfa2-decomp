/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5284_slot04_06[];

void func_801b07f8_slot04_06(Object *obj) {
    func_80142a14(obj);
}

void func_801b0818_slot04_06(Object *obj) {
    data_801c5284_slot04_06[obj->field_07](obj);
}
