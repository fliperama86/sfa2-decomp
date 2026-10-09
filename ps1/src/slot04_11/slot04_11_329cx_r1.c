/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c7514_slot04_11[];

int func_8013fd98(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_801b329c_slot04_11(Object *obj);

void func_801b329c_slot04_11(Object *obj) {
    int a;
    int b;
    u8 k;
    int lo;
    int hi;
    s16 ox;
    int n;

    a = obj->field_4c;
    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->field_10 += a;
    if (obj->field_67 != 0) {
        ref_other.p = obj->other;
        if (ref_other.p->field_163 == 0 && *(u16 *)&ref_other.p->field_04 == 0x101
            && (u8)func_8013fd98(obj, 0, 0x40, 8, 8)) {
            obj->field_4c = 0xc0000;
            obj->field_07++;
            obj->field_54 = -0x8000;
            ref_other.p->field_261 = 1;
            ((Slot04bObj *)obj)->field_52 = obj->pos_x;
            ref_other.p->pos_y = obj->pos_y;
            ref_other.p->field_45 = 0;
            if ((s16)ref_other.p->field_5c < 0) {
                obj->field_12f = 0xff;
            }
            n = obj->field_12a;
            n += 0x54;
            func_801307e0(obj, n);
            return;
        }
    }
    a = obj->pos_x;
    a -= *(s16 *)&obj->field_54;
    b = *(s16 *)((u8 *)&obj->field_54 + 2);
    a += b;
    b <<= 1;
    if (!((u32)b < (u32)a)) {
        a = data_801c7514_slot04_11[obj->field_0b];
        if (!(a & obj->field_164)) {
            func_80130efc(obj);
            return;
        }
    }
    b = -1;
    obj->field_07 += 2;
    ref_other.p = obj->other;
    lo = obj->pos_x;
    ox = ref_other.p->pos_x;
    hi = ox;
    k = obj->field_0b;
    if (k == 0) {
        int t = lo;
        lo = hi;
        hi = t;
        b = 1;
    }
    if (lo >= hi && (data_801c7514_slot04_11[k] & ref_other.p->field_164)) {
        ref_other.p->pos_x = ox + b;
        ref_other.p->field_164 = 0;
    }
    obj->field_4c = 0x80000;
    obj->field_54 = 0xffff0000;
    func_801307e0(obj, 0x59);
}
