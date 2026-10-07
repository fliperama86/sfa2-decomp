/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3274_slot04_07(Object *obj);
void func_801b3120_slot04_07(Object *obj);
void func_801b320c_slot04_07(Object *obj);

void func_801b315c_slot04_07(Object *obj) {
    func_801b3274_slot04_07(obj);
    if (((Slot04aObj *)obj)->field_1cc == 0 && obj->field_67 != 0) {
        obj->field_4c = 0x10000;
        obj->field_50 = 0x10000;
        obj->field_54 = 0;
        obj->field_58 = 0;
        ((Slot04aObj *)obj)->field_1cc++;
    }
    if ((s16)obj->field_3a & 0x8000) {
        if (obj->field_67 == 0) {
            func_801b3120_slot04_07(obj);
            return;
        }
        obj->field_07 = 5;
        obj->field_46 += 2;
    }
    func_80130efc(obj);
}

void func_801b320c_slot04_07(Object *obj) {
    int t;

    func_801b3274_slot04_07(obj);
    t = obj->field_70;
    if (obj->pos_y < t) {
        func_80130efc(obj);
    } else {
        obj->field_07 = 4;
        obj->pos_y = t;
        obj->field_45 = 0;
        func_801307e0(obj, 0x2e);
    }
}

void func_801b3274_slot04_07(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}
