/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;

void func_801b3104_slot04_08(Object *obj) {
    s16 d;
    u16 lim = 0xa0;
    Object *other;
    int k;

    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    d = (u16)obj->pos_x - data_801c3e98_slot04_08;
    if (obj->field_0b != 0) {
        d = -d;
    }
    if (d >= 0) {
        d = 2;
        if (obj->field_4c < 0) {
            d = 1;
        }
        if (obj->field_164 != d) {
            other = obj->other;
            d = -0x20;
            if (obj->field_0b != 0) {
                d = 0x20;
            }
            d = d + (u16)obj->pos_x - other->pos_x + 0x50;
            if (lim < (u16)d) {
                func_80130efc(obj);
                return;
            }
        }
    }
    obj->field_07++;
    k = 0x52;
    if (obj->field_12a != 4) {
        k = 0x51;
    }
    func_801307e0(obj, k);
}

void func_801b31f0_slot04_08(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07++;
        func_801307e0(obj, 0x53);
    } else {
        func_80130efc(obj);
    }
}
