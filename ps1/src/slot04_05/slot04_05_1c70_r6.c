/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c1528_slot04_05[];
int func_80130184(Object *object);

void func_801b22e0_slot04_05(Object *obj) {
    u16 i = 0;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 0) {
        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c1528_slot04_05[i];
    }
}

void func_801b2370_slot04_05(Object *obj) {
    if (*(u8 *)&obj->field_3a == 1) {
        obj->field_45 = 1;
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b23b0_slot04_05(Object *obj) {
    if (obj->field_50 < 0) {
        obj->field_58 = obj->field_58 - 0x2000;
    }
    if ((u8)func_80130184(obj) != 0) {
        if (*(u8 *)&obj->field_3a != 2) {
            func_80130efc(obj);
        }
    } else {
        obj->field_07++;
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        if (obj->field_0b != 0) {
            obj->field_4c = 0x70000;
        } else {
            obj->field_4c = -0x70000;
        }
        obj->field_50 = 0x30000;
        obj->field_54 = 0;
        obj->field_58 = -0x6000;
    }
}
