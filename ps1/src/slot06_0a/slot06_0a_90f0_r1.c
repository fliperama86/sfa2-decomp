/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed400_slot06_0a[];
extern ObjectFn data_801ed444_slot06_0a[];
extern void (*data_801ed454_slot06_0a[])(Object *, int, int);

void func_801e90f0_slot06_0a(Object *obj) {
    data_801ed444_slot06_0a[obj->field_04](obj);
}

void func_801e9130_slot06_0a(Object *obj) {
    obj->field_48 = 1;
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->field_81 = 4;
    obj->field_04++;
    obj->field_0c = 0;
    obj->field_4c = *(s32 *)&obj->field_10;
    obj->field_50 = 0xf80000 - *(s32 *)&obj->field_14;
    func_80130700(obj, data_801ed400_slot06_0a[2 + obj->field_03]);
}

void func_801e91a8_slot06_0a(Object *obj) {
    Slot06Layer *l = (Slot06Layer *)data_801aa5d4;

    int x = *(s32 *)&l->field_08 - *(s32 *)&l->field_20;
    int y = *(s32 *)&l->field_0c - *(s32 *)&l->field_24;

    data_801ed454_slot06_0a[obj->field_03](obj, x, y);
    obj->field_48 = obj->field_48 ^ 1;
    if (obj->field_48 != 0) {
        func_8011ffdc(obj);
    } else {
        obj->field_01 = 0;
    }
}
