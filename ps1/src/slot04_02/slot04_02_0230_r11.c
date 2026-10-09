/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6354_slot04_02[];

void func_801b2938_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b297c_slot04_02(Object *obj) {
    data_801c6354_slot04_02[obj->field_07](obj);
}
