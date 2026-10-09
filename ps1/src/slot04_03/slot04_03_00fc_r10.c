/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0658_slot04_03[];

void func_801b1c78_slot04_03(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1cbc_slot04_03(Object *obj) {
    data_801c0658_slot04_03[obj->field_07](obj);
}
