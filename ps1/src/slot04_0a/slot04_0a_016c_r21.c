/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c06f4_slot04_0a[];

void func_801b3478_slot04_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b34bc_slot04_0a(Object *obj) {
    data_801c06f4_slot04_0a[obj->field_07](obj);
}
