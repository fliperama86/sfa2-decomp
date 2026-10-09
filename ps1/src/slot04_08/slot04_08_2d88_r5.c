/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4298_slot04_08(Object *obj);
extern u16 data_801c3e98_slot04_08;

void func_801b3260_slot04_08(Object *obj) {
    int x;

    if ((s16)obj->field_3a & 0x8000) {
        obj->field_4c = 0x80000;
        obj->field_07++;
        obj->field_54 = -0xc000;
        x = 0x160;
        obj->field_67 = 0;
        if (obj->field_0b == 0) {
            x = -0x160;
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
        obj->field_46 = 0;
        data_801c3e98_slot04_08 = x + obj->pos_x;
        func_801b4298_slot04_08(obj);
    }
    func_80130efc(obj);
}

void func_801b3304_slot04_08(Object *obj) {
    s16 d;
    u16 lim = 0xa0;
    Object *other;

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
    func_801307e0(obj, 0x54);
}
