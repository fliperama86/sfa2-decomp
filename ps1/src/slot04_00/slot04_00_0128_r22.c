/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2e84_slot04_00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_0b = obj->field_0b ^ 1;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2ed4_slot04_00(Object *object) {
    u8 *p = (u8 *)object->slots;
    int i;
    u8 z = 0;

    for (i = 0x57; i >= 0; i--) {
        *p++ = z;
    }
}
