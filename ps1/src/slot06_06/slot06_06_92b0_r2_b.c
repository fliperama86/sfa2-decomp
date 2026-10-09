/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ee278_slot06_06[];
extern SequenceStep *data_801ee190_slot06_06[];
extern Slot06_06Rec83d8 data_801f83d8_slot06_06[][2];

void func_801e9c58_slot06_06(Object *obj, Slot06_06Rece1d8 *src, Slot06_06Rec83d8 *dst);

void func_801e9c58_slot06_06(Object *obj, Slot06_06Rece1d8 *src, Slot06_06Rec83d8 *dst) {
    dst->field_00 = src->field_01;
    dst->field_02 = src->field_02;
    dst->field_04 = src->field_04;
    dst->field_06 = src->field_07;
    dst->field_08 = src->field_08;
    dst->field_0a = src->field_0a;
    func_80130700(obj, data_801ee190_slot06_06[src->field_00]);
    dst->field_0c = obj->sequence;
}

void func_801e9cf8_slot06_06(Object *obj) {
    Slot06_06Rec83d8 *r;

    if ((s16)((Slot06Layer *)data_801aa5d4)->field_12 < obj->field_4c) {
        r = &data_801f83d8_slot06_06[obj->field_03][0];
    } else {
        r = &data_801f83d8_slot06_06[obj->field_03][1];
    }
    obj->sequence = r->field_0c;
    obj->field_80 = 1;
    obj->pos_x = r->field_02;
    obj->pos_y = 0xf8 - r->field_04;
    obj->field_1c = r->field_08;
    obj->field_26 = r->field_0a;
    obj->field_09 = r->field_00;
    obj->field_0e = r->field_06;
    func_80120028(obj);
}

void func_801e9db0_slot06_06(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9dd0_slot06_06(Object *obj) {
    data_801ee278_slot06_06[obj->field_04](obj);
}

void func_801e9e10_slot06_06(Object *obj) {
    u8 i = 6;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_04++;
    obj->field_0c = 0;
    obj->field_81 = 4;
    if (game_state.field_0a == 6 || game_state.field_8a != 0) {
        i = 10;
    }
    func_80130700(obj, data_801ee190_slot06_06[i + obj->field_03]);
}

void func_801e9ea0_slot06_06(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9eec_slot06_06(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
