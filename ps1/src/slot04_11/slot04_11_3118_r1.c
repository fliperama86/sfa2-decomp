/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c74f4_slot04_11[];
extern u8 data_801c74f8_slot04_11[][4];

void func_801483a4(Object *object, u16 a, u16 b);
void func_801b329c_slot04_11(Object *obj);

void func_801b3118_slot04_11(Object *obj) {
    int i;
    u8 t;

    func_80130efc(obj);
    if ((obj->field_3a & 1) == 0) {
        if (obj->field_165 != 0) {
            i = 6;
            if (obj->field_4b == 0) {
                ref_other.p = obj->other;
                ref_other.p->field_6b = 10;
                i = obj->field_12a;
            }
            t = data_801c74f8_slot04_11[i >> 1][0];
            obj->field_165 = 0;
            obj->field_27b = t;
        }
    }
    if (obj->field_3a & 2) {
        obj->field_3a &= 0xfffd;
        func_801483a4(obj, 0x28, 0x66);
        func_80120554(obj, obj->side, 0x31c);
    }
    if (obj->field_3a & 4) {
        int d;
        u16 w;

        w = obj->field_3a & 0xfffb;
        obj->field_3a = w;
        d = (w >> 8) & 0x7f;
        if (obj->field_0b == 0) {
            d = -d;
        }
        obj->pos_x += d;
    }
    if (obj->field_3a & 8) {
        obj->field_07++;
        obj->field_4c = data_801c74f4_slot04_11[obj->field_12a];
        obj->field_54 = data_801c74f4_slot04_11[obj->field_12a + 1] & 0xffff0000;
        *(u16 *)&obj->field_54 = obj->pos_x;
        func_801b329c_slot04_11(obj);
    }
}
