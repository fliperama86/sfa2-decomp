/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2034_slot04_07[];

void func_801b3274_slot04_07(Object *obj);

void func_801b2ef4_slot04_07(Object *obj) {
    int a;

    func_80130efc(obj);
    if ((u8)obj->field_3a == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->field_4c = 0xa0000;
        obj->field_54 = -0x6000;
        obj->field_50 = 0x90000;
        obj->field_07++;
        obj->field_58 = -0xa000;
        a = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c2034_slot04_07[a];
    }
}

void func_801b2fa4_slot04_07(Object *obj) {
    int y;

    func_801b3274_slot04_07(obj);
    if (obj->field_50 >= 0) {
        obj->field_07 = 5;
        obj->field_46 = 0;
    } else {
        if (((Slot04aObj *)obj)->field_1cb == 0) {
            ((Slot04aObj *)obj)->field_1cb++;
            func_801307e0(obj, 0x21);
        }
        y = obj->field_70;
        if (obj->pos_y < y) {
            func_80130efc(obj);
        } else {
            obj->field_07++;
            obj->pos_y = y;
            obj->field_45 = 0;
            obj->field_27b = 0;
            func_801307e0(obj, 0x2e);
        }
    }
}
