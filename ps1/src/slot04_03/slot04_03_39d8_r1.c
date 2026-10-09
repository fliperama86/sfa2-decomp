/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b39d8_slot04_03(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80140770(obj, 0x1c, 5, 0, 0, 0, 1);
        }
        func_80130efc(obj);
    }
}

void func_801b3a5c_slot04_03(Object *obj) {
    int z = 0;
    int i;
    u8 *p;
    u8 c;
    u8 d;

    p = (u8 *)obj->slots;
    for (c = z, i = 0x27; i >= 0; i--) {
        *p++ = c;
    }
    p = &((Slot04aObj *)obj)->field_184;
    for (d = z, i = 0x17; i >= 0; i--) {
        *p++ = d;
    }
}
