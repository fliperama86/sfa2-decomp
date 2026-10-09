/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ea6e4_slot06_0f(Object *obj);
int func_801ea77c_slot06_0f(Object *obj);
int func_801ea7e4_slot06_0f(Object *obj);
void func_801ea91c_slot06_0f(Object *obj, int idx);
void func_801ea96c_slot06_0f(Object *obj);

extern ObjectFn data_801eeffc_slot06_0f[];

void func_801ea618_slot06_0f(Object *obj) {
    data_801eeffc_slot06_0f[obj->field_06](obj);
}

void func_801ea658_slot06_0f(Object *obj) {
    int idx;

    if (game_state.config->field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
        idx = 5;
        if (*(s32 *)&game_state.config->field_78 == (s32)game_state.field_358) {
            func_801204f4(obj, obj->field_66, 0x12);
            idx = 4;
        }
        func_801ea91c_slot06_0f(obj, idx);
    }
    func_801ea6e4_slot06_0f(obj);
}

void func_801ea6e4_slot06_0f(Object *obj) {
    func_80131094(obj);
}

void func_801ea704_slot06_0f(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 0;
    obj->field_07 = 0;
    func_801ea96c_slot06_0f(obj);
}

void func_801ea734_slot06_0f(Object *obj) {
    int idx = (s16)obj->pos_x > (s16)game_state.field_358->pos_x;

    if (obj->field_48 != idx) {
        obj->field_48 = idx;
        func_801ea91c_slot06_0f(obj, idx);
    }
}

int func_801ea77c_slot06_0f(Object *obj) {
    s16 v = *(s16 *)&game_state.field_358->other->field_04;

    if (v != 0x101) {
        return v == 0x103;
    }
    return 1;
}

void func_801ea7b4_slot06_0f(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 1;
    obj->field_06 = 0;
    obj->field_07 = 0;
    func_801ea96c_slot06_0f(obj);
}

int func_801ea7e4_slot06_0f(Object *obj) {
    s16 v = *(s16 *)&game_state.field_358->field_04;

    if (v == 0x101) {
        if (obj->field_61 != 0xff) {
            return 1;
        }
    } else if (v == 0x103) {
        return 1;
    }
    return 0;
}

void func_801ea828_slot06_0f(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 2;
    obj->field_06 = 0;
    obj->field_07 = 0;
    func_801ea96c_slot06_0f(obj);
}

int func_801ea85c_slot06_0f(Object *obj) {
    if (func_801ea77c_slot06_0f(obj)) {
        return func_801ea7e4_slot06_0f(obj) != 0;
    }
    return 0;
}

void func_801ea898_slot06_0f(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 1;
    obj->field_06 = 0;
    obj->field_07 = 0;
    func_801ea96c_slot06_0f(obj);
}

int func_801ea8c8_slot06_0f(Object *obj) {
    return game_state.config->field_47 != 0;
}

void func_801ea8e0_slot06_0f(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 3;
    obj->field_06 = 0;
    obj->field_07 = 0;
}

void func_801ea8fc_slot06_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
