/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;
extern u8 data_801903c4[];
extern void (*data_800f01ac_slot0f[])(Object *);
void func_800e79a0_slot0f(Object *obj);
void func_800e78e0_slot0f(Object *obj);
void func_800e8064_slot0f(Object *obj, int arg);

void func_800e7508_slot0f(Object *obj) {
    obj->field_05 = obj->field_05 + 1;
    func_800e79a0_slot0f(obj);
}

void func_800e7534_slot0f(Object *obj) {
    data_800f01ac_slot0f[obj->field_05](obj);
}

void func_800e7574_slot0f(Object *obj) {
    obj->field_46 = 0x1e;
    obj->field_05++;
    if (data_80190468.p->field_01 != 0) {
        obj->field_05 = 3;
    }
    func_800e78e0_slot0f(obj);
    func_800e8064_slot0f(obj, 0);
    data_801903c4[0] = 1;
}
