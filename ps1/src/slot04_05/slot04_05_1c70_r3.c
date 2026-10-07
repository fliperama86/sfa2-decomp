/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801aa5e4[];
extern s32 data_801c14c0_slot04_05[];
extern s32 data_801c14d8_slot04_05[];
int func_80130184(Object *object);

void func_801b1e78_slot04_05(Object *obj) {
    int a;
    int b;
    s32 v = data_801aa5e4[0];

    *(s32 *)&obj->field_10 = v;
    if (obj->field_0b == 0) {
        *(s32 *)&obj->field_10 = v + 0x1800000;
    }
    if (*(u8 *)&obj->field_3a == 1) {
        obj->field_48 = 1;
        obj->field_07++;
        obj->field_54 = 0;
        obj->field_58 = 0;
        a = data_801c14c0_slot04_05[obj->field_12a];
        b = data_801c14c0_slot04_05[obj->field_12a + 1];
        if (obj->field_49 != 0) {
            a = data_801c14d8_slot04_05[obj->field_12a];
            b = data_801c14d8_slot04_05[obj->field_12a + 1];
        }
        if (obj->field_0b == 0) {
            a = -a;
        }
        obj->field_4c = a;
        obj->field_50 = b;
    }
    func_80130efc(obj);
}

void func_801b1f54_slot04_05(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        if (*(u8 *)&obj->field_3a != 2) {
            func_80130efc(obj);
        }
    } else {
        obj->field_07++;
        func_801209c4(obj);
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        obj->field_17b = 0;
    }
}
