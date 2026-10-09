/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 func_801b1e04_slot04_0a(Object *obj);

void func_801b30a0_slot04_0a(Object *obj) {
    obj->field_07 = 4;
    func_801209c4(obj);
    obj->field_14 = 0;
    obj->pos_y = ((Slot04aObj *)obj)->field_70;
    func_801307e0(obj, 0x48);
}

void func_801b30e8_slot04_0a(Object *obj) {
    u32 t;

    if (func_801b1e04_slot04_0a(obj)) {
        func_801b30a0_slot04_0a(obj);
    } else {
        if (((Slot04aObj *)obj)->field_3a == 0 && obj->field_cd == 0) {
            t = obj->field_130 & 0xa000;
            if (t != 0) {
                u32 m = t & 0x8000;

                t = 0xfffc0000;
                if (m == 0) {
                    t = 0x40000;
                }
                if (obj->field_0b != 0) {
                    t = -t;
                }
                *(s32 *)&obj->field_10 = t + *(s32 *)&obj->field_10;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b3198_slot04_0a(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}
