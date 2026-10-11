/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801ce050_slot04_17[];
extern s32 data_801ce3e4_slot04_17[];

void func_801b27a0_slot04_17(Object *obj) {
    int s;
    s16 w;
    int d;
    int vx;
    int vy;
    int vz;
    int vw;
    int q;
    /* Stand-in, not recovered source: the local k, which holds the constant 0x2000 for one test, and the local q, which keeps the masked value apart from its 16-bit copy w, reproduce the original's choice of registers. Without it this function differs from the original in 3 instruction slots. */
    /* One stand-in of two locals: with either taken out alone (the test written with the literal, or w set from the mask directly) the figure is 3 as well. Inferred: with the plain forms the compiler tests the unmasked value. */
    int k = 0x2000;

    if ((obj->field_3a << 16) < 0) {
        if (obj->field_cd != 0) {
            if (func_8014a170(obj, data_801ce050_slot04_17) != 0) {
                goto act_b;
            }
            goto act_c;
        } else {
            s = obj->field_130;
            if ((s & 0x1000) == 0) {
                goto act_c;
            }
        }
act_b:
        d = obj->field_4c >> 2;
        obj->field_07 = 7;
        obj->field_50 = 0x10000;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
        obj->field_4c = d;
        func_801307e0(obj, 0x34);
        return;
act_c:
        obj->field_07++;
        func_801204f4(obj, obj->side, 6);
        if (obj->field_cd != 0) {
            ref_other.p = obj->other;
            d = ref_other.p->pos_x - obj->pos_x;
            s = -0x8000;
            if (d >= 0) {
                s = 0x2000;
            }
            if (obj->field_0b != 0) {
                s ^= 0xa000;
            }
            d += 0x10;
            if (d < 0x21) {
                s = 0;
            }
        }
        d = 1;
        vx = 0;
        vz = 0;
        vy = -0x8000;
        q = s & -0x6000;
        w = q;
        vw = -0x8000;
        if (w != 0) {
            d = 0;
            vx = 0x28000;
            vz = 0;
            vy = -0x8000;
            vw = -0x6000;
            if ((w & data_801ce3e4_slot04_17[obj->field_0b]) == 0) {
                vx = -0x28000;
            }
            if ((q & k) != 0) {
                obj->field_0b ^= 1;
            }
        }
        obj->field_48 = d;
        d += 0x2e;
        if (obj->field_49 != 0) {
            vx <<= 1;
            vy <<= 1;
        }
        obj->field_4c = vx;
        obj->field_54 = vz;
        obj->field_50 = vy;
        obj->field_58 = vw;
        func_801307e0(obj, d + obj->field_12a);
    } else {
        func_80130efc(obj);
    }
}
