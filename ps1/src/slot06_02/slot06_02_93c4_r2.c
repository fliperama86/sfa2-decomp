/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ef0d8_slot06_02[];
extern SequenceStep *data_801ef0c8_slot06_02[];

void func_801e9d24_slot06_02(Object *obj) {
    data_801ef0d8_slot06_02[obj->field_04](obj);
}

void func_801e9d64_slot06_02(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_81 = 4;
    obj->field_04++;
    obj->field_0c = 0;
    obj->field_4c = *(s32 *)&obj->field_10;
    obj->field_50 = 0x1000000 - *(s32 *)&obj->field_14;
    func_80130700(obj, data_801ef0c8_slot06_02[obj->field_03]);
}

void func_801e9dd8_slot06_02(Object *obj) {
    Slot06Layer *l = (Slot06Layer *)cam_obj;
    s32 a;
    s32 b;

    a = *(s32 *)&l->field_20 - *(s32 *)&l->field_08;
    b = a;
    a >>= 2;
    a += b;
    *(s32 *)&obj->field_10 = obj->field_4c - a;
    a = *(s32 *)&l->field_14 - *(s32 *)&l->field_0c;
    b = a >> 2;
    a += b;
    *(s32 *)&obj->field_14 = 0xf80000 - (obj->field_50 - a);
    func_8011ffdc(obj);
}

void func_801e9e54_slot06_02(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
