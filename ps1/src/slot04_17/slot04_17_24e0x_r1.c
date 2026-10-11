/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801ce050_slot04_17[];
extern s32 data_801ce384_slot04_17[];

int func_801b56e0_slot04_17(Object *obj);
Object *func_801b5890_slot04_17(Object *obj);

void func_801b24e0_slot04_17(Object *obj) {
    int t;
    s32 a;
    int s;

    func_801b56e0_slot04_17(obj);
    if ((obj->field_3a << 16) < 0) {
        if (obj->field_cd != 0) {
            if (func_8014a170(obj, data_801ce050_slot04_17) != 0) {
                goto act_b;
            }
            if ((func_80151184() & 1) == 0) {
                goto act_a;
            }
            goto act_c;
        } else {
            t = obj->field_130;
            s = t & 0xf000;
            if ((t & 0x1000) == 0) {
                goto not_a;
            }
        }
act_a:
        obj->field_07++;
        ref_other.p = obj->other;
        a = *(s32 *)&ref_other.p->field_10 - *(s32 *)&obj->field_10;
        obj->field_54 = 0;
        obj->field_58 = 0;
        a >>= 4;
        obj->field_4c = a;
        a = obj->field_70 - 0x8a;
        a <<= 16;
        a -= *(s32 *)&obj->field_14;
        a >>= 4;
        a -= 0x4000;
        obj->field_50 = -a;
        func_801307e0(obj, 0x2c);
        return;
not_a:
        if ((t & 0x2000) == 0) {
            goto act_c;
        }
act_b:
        a = 0x18000;
        obj->field_07 = 7;
        if (obj->field_0b == 0) {
            a = -0x18000;
        }
        obj->field_50 = 0x10000;
        obj->field_4c = a;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
        func_801307e0(obj, 0x34);
        return;
act_c:
        obj->field_07 = 6;
        obj->field_48 = 0;
        if (obj->field_cd != 0) {
            func_801b5890_slot04_17(obj);
            s = 0x8000;
            if (obj->field_0b != 0) {
                s = 0;
                /* Stand-in, not recovered source: this assignment, which nothing reads, reproduces the original's order of instructions. Without it this function differs from the original in 8 instruction slots. */
                t = 0;
            }
        }
        {
            int i = obj->field_12a << 1;
            s32 v0, v1, v2;

            a = 0x26;
            if ((s & 0x8000) != 0) {
                a = 0x27;
                i += 0xc;
            }
            v0 = data_801ce384_slot04_17[i];
            v1 = data_801ce384_slot04_17[i + 1];
            v2 = data_801ce384_slot04_17[i + 2];
            /* Stand-in, not recovered source: the local i, used again for the fourth table value, reproduces the original's choice of registers. Without it this function differs from the original in 17 instruction slots. */
            i = data_801ce384_slot04_17[i + 3];
            if (obj->field_0b == 0) {
                v0 = -v0;
            }
            if (obj->field_49 != 0) {
                v0 <<= 1;
                v2 <<= 1;
            }
            obj->field_4c = v0;
            obj->field_54 = v1;
            obj->field_50 = v2;
            obj->field_58 = i;
            func_801307e0(obj, a + obj->field_12a);
        }
    } else {
        func_80130efc(obj);
    }
}
