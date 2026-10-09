/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b48fc_slot04_17(Object *obj);
u8 func_801b4974_slot04_17(Object *obj);

extern u32 data_801ce0d0_slot04_17[];
extern s32 data_801ce63c_slot04_17[];

void func_801b45ec_slot04_17(Object *obj) {
    int a;
    int d;
    int m;
    int n;
    int j;
    int s;
    s32 x;
    s32 y;

    func_801b48fc_slot04_17(obj);
    if ((obj->field_3a << 16) < 0) {
        a = *(u16 *)&((Slot04bObj *)obj)->field_1c0;
        obj->field_07++;
        if (obj->field_cd != 0) {
            if (func_8014a170(obj, data_801ce0d0_slot04_17) == 0) {
                ref_other.p = obj->other;
                d = ref_other.p->pos_x - obj->pos_x;
                a = 0x40;
                if (d >= 0) {
                    a = 8;
                }
                d += 0x10;
                if (d < 0x21) {
                    a = 0x20;
                }
            } else {
                a = func_801b4974_slot04_17(obj);
            }
        }
        d = 1;
        a &= 0x68;
        if (a != 0) {
            if ((a & 0x20) == 0) {
                d = 0;
                obj->field_0b = 0;
                if ((a & 0x40) == 0) {
                    obj->field_0b = 1;
                }
            }
        }
        /* The locals j, n, m and s fix the order of these sums. With d << 2 written at its use five instruction slots differ, with m + n at its use twelve, with field_12e++ eight. */
        j = d << 2;
        n = obj->field_12e + 8;
        m = obj->field_12a * 6;
        *(u16 *)&((Slot04bObj *)obj)->field_1c0 = 0;
        obj->field_12e = obj->field_12e + 1;
        a = j + m;
        x = data_801ce63c_slot04_17[a];
        y = data_801ce63c_slot04_17[a + 1];
        obj->field_50 = data_801ce63c_slot04_17[a + 2];
        obj->field_58 = data_801ce63c_slot04_17[a + 3];
        s = m + n;
        if (obj->field_0b == 0) {
            x = -x;
            y = -y;
        }
        obj->field_4c = x;
        obj->field_54 = y;
        func_801307e0(obj, d + data_801ce63c_slot04_17[s]);
    } else {
        func_80130efc(obj);
    }
}
