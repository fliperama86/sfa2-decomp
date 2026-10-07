/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c0548_slot04_03[];

void func_801b06a0_slot04_03(Object *obj) {
    s16 t = obj->field_3a;
    s16 x;
    int d;

    if ((t & 0x8000) != 0) {
        func_801312b8(obj);
    } else {
        x = t & 0xff;
        if (x == (s16)obj->field_46) {
            if ((t & 0xff00) == 0) {
                func_80130efc(obj);
                return;
            }
        } else {
            obj->field_46 = x;
        }
        if (x == 0x16) {
            obj->field_3a = 0;
        }
        d = data_801c0548_slot04_03[x >> 2];
        if (obj->field_48 != 0) {
            d = -d;
        }
        *(s32 *)&obj->field_10 = d + *(s32 *)&obj->field_10;
        func_80130efc(obj);
    }
}

void func_801b075c_slot04_03(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_07++;
    }
    func_80130efc(obj);
}
