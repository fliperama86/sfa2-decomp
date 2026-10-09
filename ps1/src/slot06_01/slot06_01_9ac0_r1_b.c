/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea998_slot06_01[];
extern SequenceStep *data_801eb984_slot06_01[];
extern SequenceStep *data_801eb9ac_slot06_01;

void func_801e9d78_slot06_01(Object *obj) {
    if (player_left.field_06 == 5 || player_right.field_06 == 8) {
        obj->field_05++;
        func_80130700(obj, data_801eb9ac_slot06_01);
    } else {
        if ((s16)obj->field_3a & 0x8000) {
            obj->field_05 = 1;
        }
        func_80131094(obj);
    }
}

void func_801e9dfc_slot06_01(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = 1;
    }
    func_80131094(obj);
}

void func_801e9e34_slot06_01(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9e54_slot06_01(Object *obj) {
    data_801ea998_slot06_01[obj->field_04](obj);
}

void func_801e9e94_slot06_01(Object *obj) {
    SequenceStep *s;
    u16 y;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_76 = 0x300;
    obj->field_78 = 0x100;
    obj->field_7a = 0x60;
    obj->field_7c = 0x1e0;
    obj->field_04++;
    obj->field_0d = 0;
    obj->field_81 = 4;
    y = obj->pos_y;
    obj->pos_y = 0xf8 - y;
    func_80130700(obj, data_801eb984_slot06_01[0]);
    func_8011ffdc(obj);
}

void func_801e9f24_slot06_01(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9f70_slot06_01(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
