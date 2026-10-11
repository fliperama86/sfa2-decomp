/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800f01ac_slot0f[])(Object *);
void func_800e79a0_slot0f(Object *obj);
void func_800e78e0_slot0f(Object *obj);
void func_800e8064_slot0f(Object *obj, int arg);

void func_800e7508_slot0f(Object *obj) {
    obj->state_1 = obj->state_1 + 1;
    func_800e79a0_slot0f(obj);
}

void func_800e7534_slot0f(Object *obj) {
    data_800f01ac_slot0f[obj->state_1](obj);
}

void func_800e7574_slot0f(Object *obj) {
    obj->field_46 = 0x1e;
    obj->state_1++;
    if (data_80190468.p->field_01 != 0) {
        obj->state_1 = 3;
    }
    func_800e78e0_slot0f(obj);
    func_800e8064_slot0f(obj, 0);
    data_801903c4[0] = 1;
}
