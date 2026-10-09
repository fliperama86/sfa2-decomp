/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5250_slot04_06[];

void func_801b04b0_slot04_06(Object *obj) {
    func_80142a14(obj);
}

void func_801b04d0_slot04_06(Object *obj) {
    data_801c5250_slot04_06[obj->field_07](obj);
}
