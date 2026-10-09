/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c266c_slot04_0b[];

void func_801b316c_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b31b0_slot04_0b(Object *obj) {
    data_801c266c_slot04_0b[obj->field_07](obj);
}
