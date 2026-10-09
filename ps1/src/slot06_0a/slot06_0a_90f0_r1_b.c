/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed400_slot06_0a[];
extern ObjectFn data_801ed460_slot06_0a[];
extern ObjectFn data_801ed470_slot06_0a[];
extern ObjectFn data_801ed480_slot06_0a[];

void func_801e96cc_slot06_0a(Object *obj);
void func_801e9720_slot06_0a(Object *obj);
void func_801e95a8_slot06_0a(Object *obj);
void func_801e97c0_slot06_0a(Object *obj);

void func_801e9284_slot06_0a(Object *obj, int a, int b) {
    int t = a >> 1;
    int u = b >> 1;
    a = (a >> 6) + t;
    b = (b >> 4) + u;
    a += obj->field_4c;
    b += obj->field_50;
    *(s32 *)&obj->field_10 = a;
    *(s32 *)&obj->field_14 = 0xf80000 - b;
}

void func_801e92c0_slot06_0a(Object *obj, int a, int b) {
    a = a - (a >> 4);
    a += obj->field_4c;
    b += obj->field_50;
    *(s32 *)&obj->field_10 = a;
    *(s32 *)&obj->field_14 = 0xf80000 - b;
}

void func_801e92ec_slot06_0a(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e930c_slot06_0a(Object *obj) {
    data_801ed460_slot06_0a[obj->field_04](obj);
}

void func_801e934c_slot06_0a(Object *obj) {
    int i = 7;
    Object *a = &player_left;
    Object *b = a + 1;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_81 = 4;
    obj->field_04++;
    obj->field_0c = 0;
    if (obj->field_03 != 0) {
        i = 0xd;
        a = b;
        b = a - 1;
    }
    if (a->kind == 0xa) {
        obj->field_4c = (s32)a;
    } else if (b->kind == 0xa) {
        obj->field_4c = (s32)b;
    } else {
        obj->field_4c = 0;
    }
    func_80130700(obj, data_801ed400_slot06_0a[i]);
}

void func_801e93fc_slot06_0a(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->field_03 != 0) {
            func_801e95a8_slot06_0a(obj);
            return;
        }
        data_801ed470_slot06_0a[obj->field_05](obj);
    }
    func_8011ffdc(obj);
}

void func_801e9488_slot06_0a(Object *obj) {
    func_801e9720_slot06_0a(obj);
    func_801e96cc_slot06_0a(obj);
    func_80131094(obj);
}

void func_801e94c0_slot06_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = 2;
        func_80130700(obj, data_801ed400_slot06_0a[5]);
    } else {
        func_80131094(obj);
    }
}

void func_801e9510_slot06_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801e97c0_slot06_0a(obj);
    }
    func_80131094(obj);
}

void func_801e9558_slot06_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = 0;
        func_80130700(obj, data_801ed400_slot06_0a[7]);
    } else {
        func_80131094(obj);
    }
}

void func_801e95a8_slot06_0a(Object *obj) {
    data_801ed480_slot06_0a[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801e95fc_slot06_0a(Object *obj) {
    func_801e9720_slot06_0a(obj);
    func_80131094(obj);
}

void func_801e962c_slot06_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = 2;
        func_80130700(obj, data_801ed400_slot06_0a[0xa]);
    } else {
        func_80131094(obj);
    }
}

void func_801e967c_slot06_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = 0;
        func_80130700(obj, data_801ed400_slot06_0a[0xd]);
    } else {
        func_80131094(obj);
    }
}

void func_801e96cc_slot06_0a(Object *obj) {
    if (player_left.field_06 == 8 || player_right.field_06 == 8) {
        obj->field_05 = 3;
        func_80130700(obj, data_801ed400_slot06_0a[8]);
    }
}
