/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801ce44c_slot04_17[];
extern s16 data_801ce454_slot04_17[];

int func_80140cd8(Object *object, int a, int b);
void func_801b3784_slot04_17(Object *obj);

void func_801b34b8_slot04_17(Object *obj) {
    Object *p;
    int a;
    int j;
    int i;
    int y;
    u16 t;

    a = obj->vel_x;
    p = obj->other;
    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s32)obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    t = obj->step_flags;
    a = t & 0xff;
    if (a != 0) {
        if (a & 0x80) {
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            /* Without it (plain braces) this function differs from the original in 34 instruction slots. */
            do {
                obj->state_3++;
                p = obj->other;
                p->field_6a++;
                func_801465b0(obj, 6, 4, 0x3c);
                p = obj->other;
                p->field_15b = 1;
                i = obj->field_12a;
                func_80140770(obj, 0x10, 5, data_801ce44c_slot04_17[i], data_801ce44c_slot04_17[i + 1], 0, 0);
            } while (0);
            p = obj->other;
            if ((s16)p->field_5c < 0) {
                func_80120554(p, p->side, 0x339);
                func_801b3784_slot04_17(obj);
            } else {
                func_80120554(p, p->side, 0x306);
            }
            return;
        }
        a -= 8;
        obj->step_flags = t >> 8;
        ref_other.p = obj->other;
        ref_other.p->field_6a++;
        j = a >> 1;
        y = data_801ce454_slot04_17[j + 3];
        /* Stand-in, not recovered source: the local i, the index of the other arm, used here for the table's third halfword, reproduces the original's choice of registers. Without it this function differs from the original in 13 instruction slots. */
        i = data_801ce454_slot04_17[j + 2];
        func_801465b0(obj, 6, data_801ce454_slot04_17[j], data_801ce454_slot04_17[j + 1]);
        if (func_80140cd8(obj, i, y) != 0) {
            func_80120554(p, p->side, 0x339);
            func_801b3784_slot04_17(obj);
        } else {
            p = obj->other;
            func_80120554(p, p->side, 0x306);
        }
    }
    func_80130efc(obj);
}
