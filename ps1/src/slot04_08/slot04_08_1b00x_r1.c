/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;

void func_801b4298_slot04_08(Object *obj);

void func_801b1b00_slot04_08(Object *obj) {
    u16 t;
    u16 a;
    unsigned lim;
    Object *p;
    int v;
    u8 f;
    int n;
    u16 x;

    t = (obj->field_46 + 1) & 7;
    obj->field_46 = t;
    if (t == 0) {
        func_801b4298_slot04_08(obj);
    }
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    x = *(u16 *)&obj->pos_x;
    f = obj->field_0b;
    v = x - data_801c3e98_slot04_08;
    a = v;
    if (f != 0) {
        a = -v;
    }
    if ((s16)a >= 0) {
        a = 2;
        if (obj->field_4c < 0) {
            a = 1;
        }
        if (obj->field_164 != (s16)a) {
            p = obj->other;
            v = -0x20;
            a = v;
            if (f != 0) {
                a = 0x20;
            }
            a = (a + x - *(u16 *)&p->pos_x) + 0x40;
            lim = 0x80;
            if (lim < (u16)a) {
                func_80130efc(obj);
            } else {
                goto tail;
            }
        } else {
            goto tail;
        }
    } else {
tail:
        n = 0x29;
        ((Slot04aObj *)obj)->field_1c8 = 8;
        obj->field_07++;
        if (obj->field_49 != 0) {
            n = 0x71;
        }
        func_801307e0(obj, (obj->field_12a >> 1) + n);
    }
}
