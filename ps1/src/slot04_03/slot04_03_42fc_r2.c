/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c0a08_slot04_03[];

void func_801b445c_slot04_03(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            if (func_80149b80(obj) & 0xff) {
                obj->field_07 = 0;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b44d0_slot04_03(Object *obj) {
    s16 t = obj->field_3a;
    s16 a;
    int v;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        a = t & 0xff;
        if (a == (s16)obj->field_46) {
            if ((t & 0xff00) == 0) {
                func_80130efc(obj);
                return;
            }
        } else {
            obj->field_46 = a;
        }
        v = *(s32 *)((u8 *)data_801c0a08_slot04_03 + (a & 0xfc));
        if (obj->field_0b == 0) {
            v = -v;
        }
        *(s32 *)&obj->field_10 += v;
        func_80130efc(obj);
    }
}
