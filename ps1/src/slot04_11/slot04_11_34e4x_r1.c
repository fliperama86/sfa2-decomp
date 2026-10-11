/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7518_slot04_11[];
extern s16 data_801c7520_slot04_11[];

int func_80140cd8(Object *object, int a, int b);
void func_801b37f8_slot04_11(Object *obj);

void func_801b34e4_slot04_11(Object *obj) {
    int a;
    int i;
    int j;
    int y;
    u16 t;

    a = obj->field_4c;
    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->field_10 = a + *(s32 *)&obj->field_10;
    obj->field_4c = obj->field_4c + obj->field_54;
    if ((s32)obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    t = obj->field_3a;
    a = t & 0xff;
    if (a != 0) {
        if (a & 0x80) {
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            do {
                obj->field_07++;
                ref_other.p = obj->other;
                ref_other.p->field_6a++;
                func_801465b0(obj, 6, 4, 0x3c);
                ref_other.p = obj->other;
                ref_other.p->field_15b = 1;
                i = obj->field_12a;
                func_80140770(obj, 0x10, 5, data_801c7518_slot04_11[i], data_801c7518_slot04_11[i + 1], 0, 0);
            } while (0);
            ref_other.p = obj->other;
            if ((s16)ref_other.p->field_5c < 0) {
                func_80120554(ref_other.p, ref_other.p->side, 0x339);
                func_801b37f8_slot04_11(obj);
            } else {
                func_80120554(ref_other.p, ref_other.p->side, 0x306);
            }
            return;
        }
        a -= 8;
        obj->field_3a = t >> 8;
        ref_other.p = obj->other;
        ref_other.p->field_6a++;
        j = a >> 1;
        y = data_801c7520_slot04_11[j + 3];
        i = data_801c7520_slot04_11[j + 2];
        func_801465b0(obj, 6, data_801c7520_slot04_11[j], data_801c7520_slot04_11[j + 1]);
        ref_other.p = obj->other;
        if (func_80140cd8(obj, i, y) != 0) {
            func_80120554(ref_other.p, ref_other.p->side, 0x339);
            func_801b37f8_slot04_11(obj);
        } else {
            func_80120554(ref_other.p, ref_other.p->side, 0x306);
        }
    }
    func_80130efc(obj);
}
