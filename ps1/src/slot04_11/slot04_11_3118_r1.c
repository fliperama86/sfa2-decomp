/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c74f4_slot04_11[];
extern u8 data_801c74f8_slot04_11[][4];

void func_801483a4(Object *object, int a, int b);
void func_801b329c_slot04_11(Object *obj);

void func_801b3118_slot04_11(Object *obj) {
    int i;
    u8 t;

    func_80130efc(obj);
    if ((obj->step_flags & 1) == 0) {
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
    if (obj->step_flags & 2) {
        obj->step_flags &= 0xfffd;
        func_801483a4(obj, 0x28, 0x66);
        func_80120554(obj, obj->side, 0x31c);
    }
    if (obj->step_flags & 4) {
        int d;
        u16 w;

        w = obj->step_flags & 0xfffb;
        obj->step_flags = w;
        d = (w >> 8) & 0x7f;
        if (obj->field_0b == 0) {
            d = -d;
        }
        obj->pos_x += d;
    }
    if (obj->step_flags & 8) {
        obj->state_3++;
        obj->vel_x = data_801c74f4_slot04_11[obj->field_12a];
        obj->accel_x = data_801c74f4_slot04_11[obj->field_12a + 1] & 0xffff0000;
        *(u16 *)&obj->accel_x = obj->pos_x;
        func_801b329c_slot04_11(obj);
    }
}
