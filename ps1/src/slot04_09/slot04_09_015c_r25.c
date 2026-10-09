/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7dc4_slot04_09[];

void func_801b47c8_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b480c_slot04_09(Object *obj) {
    data_801c7dc4_slot04_09[obj->field_07](obj);
}
