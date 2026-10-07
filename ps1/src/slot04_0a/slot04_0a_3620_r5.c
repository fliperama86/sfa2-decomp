/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3ba0_slot04_0a(Object *obj) {
    func_80130efc(obj);
    if ((u8)obj->field_3a == 2) {
        obj->field_07++;
        obj->field_0b ^= 1;
    }
}

void func_801b3bf0_slot04_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_73 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
