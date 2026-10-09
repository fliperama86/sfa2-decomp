/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;

void func_801b16b0_slot04_08(Object *obj) {
    u16 t;
    s16 a;
    Object *p;
    Object *q;
    unsigned lim;
    int v;
    int n;
    u16 x;

    t = (obj->field_46 + 1) & 7;
    obj->field_46 = t;
    if (t == 0) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 8;
            p->field_03 = 1;
            p->field_3c = obj;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y - 0x6c;
            p->field_7a = obj->field_7a;
            p->field_7c = obj->field_7c;
            p->field_0d = obj->field_0d;
            p->field_08 = 0x20;
            p->field_66 = obj->field_66;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
        }
    }
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    x = *(u16 *)&obj->pos_x;
    v = x - data_801c3e98_slot04_08;
    a = v;
    if (obj->field_0b != 0) {
        a = (u16)-v;
    }
    if ((s16)a >= 0) {
        a = 2;
        if (obj->field_4c < 0) {
            a = 1;
        }
        if (obj->field_164 != (s16)a) {
            q = obj->other;
            a = -0x20;
            if (obj->field_0b != 0) {
                a = 0x20;
            }
            a = (a + x - *(u16 *)&q->pos_x) + 0x40;
            lim = 0x80;
            if (lim < (u16)a) {
                func_80130efc(obj);
                return;
            }
        }
    }
    obj->field_07++;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x80000;
    } else {
        obj->field_4c = -0x80000;
    }
    n = 0x20;
    ((Slot04aObj *)obj)->field_1c8 = 8;
    if (obj->field_49 != 0) {
        n = 0x68;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + n);
}
