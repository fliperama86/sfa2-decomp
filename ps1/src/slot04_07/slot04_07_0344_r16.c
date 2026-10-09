/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c20d4_slot04_07[];

void func_801b3e48_slot04_07(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3e8c_slot04_07(Object *obj) {
    data_801c20d4_slot04_07[obj->field_07](obj);
}
