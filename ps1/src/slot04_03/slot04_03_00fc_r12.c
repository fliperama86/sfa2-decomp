/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2840_slot04_03(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2884_slot04_03(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
