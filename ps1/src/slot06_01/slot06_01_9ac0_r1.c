/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea95c_slot06_01[];
extern u8 data_801ea964_slot06_01[];
extern ObjectFn data_801ea988_slot06_01[];
extern SequenceStep *data_801eb960_slot06_01[];

void func_801e9c58_slot06_01(Object *obj);

void func_801e9ac0_slot06_01(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->field_58 != 0) {
            func_801e9c58_slot06_01(obj);
            return;
        }
        data_801ea95c_slot06_01[obj->field_05](obj);
    }
    func_8011ffdc(obj);
}

void func_801e9b4c_slot06_01(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        u8 v;
        obj->field_05++;
        if (obj->field_54 == 3) {
            v = 0xb;
        } else if (obj->field_54 == 8) {
            v = 7;
        } else {
            v = func_80151184();
        }
        obj->field_46 = v;
    }
    func_80131094(obj);
}

void func_801e9bc8_slot06_01(Object *obj) {
    int v = obj->field_46 - 1;
    obj->field_46 = v;
    if ((s16)v == 0) {
        obj->field_05 = 0;
        obj->field_54 = data_801ea964_slot06_01[(u8)((func_80151184() & 3) | ((u8)obj->field_54 << 2))];
        func_80130700(obj, data_801eb960_slot06_01[obj->field_54]);
    }
}

void func_801e9c58_slot06_01(Object *obj) {
    data_801ea988_slot06_01[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801e9cac_slot06_01(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = obj->field_05 + 1;
    }
    func_80131094(obj);
}
