/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801bfea8_slot04_00[];

void func_801b27bc_slot04_00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2800_slot04_00(Object *obj) {
    data_801bfea8_slot04_00[obj->field_07](obj);
}
