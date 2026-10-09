/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eb8dc_slot06_0e[];

void func_801e99ec_slot06_0e(Object *obj);
void func_801e9ab0_slot06_0e(Object *obj);
int func_801e9af8_slot06_0e(Object *obj);
void func_801e9bc8_slot06_0e(Object *obj);
void func_801e9c44_slot06_0e(Slot06Pal *pal);

void func_801e99ac_slot06_0e(Object *obj) {
    data_801eb8dc_slot06_0e[obj->field_04](obj);
}

void func_801e99ec_slot06_0e(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    Slot06Pal *p0 = (Slot06Pal *)((u8 *)obj + 0x30);
    Slot06Pal *p1 = (Slot06Pal *)((u8 *)obj + 0x40);
    Slot06Pal *p2 = (Slot06Pal *)((u8 *)obj + 0x50);
    Slot06Pal *p3 = (Slot06Pal *)((u8 *)obj + 0x6c);
    obj->field_3c = 3;
    obj->field_4c = 4;
    obj->field_5c = 5;
    obj->field_78 = 6;
    obj->field_38 = 0x150;
    obj->field_48 = 0x1a0;
    obj->field_58 = 0x330;
    obj->field_74 = 0x370;
    obj->field_01 = 0;
    obj->field_3a = 0x50;
    obj->field_4a = 0;
    obj->field_5a = 0;
    obj->field_76 = 0x50;
    obj->field_04 = obj->field_04 + 1;
    func_801e9c44_slot06_0e(p0);
    func_801e9c44_slot06_0e(p1);
    func_801e9c44_slot06_0e(p2);
    func_801e9c44_slot06_0e(p3);
}

void func_801e9ab0_slot06_0e(Object *obj) {
    if (obj->field_05 == 0) {
        func_801e9af8_slot06_0e(obj);
    } else if (obj->field_05 == 1) {
        func_801e9bc8_slot06_0e(obj);
    }
}
