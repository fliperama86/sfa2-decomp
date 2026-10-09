/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051bd0_slot28[];
extern ObjectRef data_80051bd4_slot28;
void func_80022ccc_slot28(void);
void func_80022c3c_slot28(Object *obj, int arg);

void func_80022810_slot28(Object *obj) {
    int t = data_8018f5a0->field_60 - 1;
    data_8018f5a0->field_60 = t;
    if ((s16)t < 0) {
        func_8014f4d4(6, 4);
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_801282d4();
    }
}

void func_80022874_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_80022ccc_slot28();
        func_80128370();
        data_80051bd0_slot28[0]->field_01 = 0;
        data_80051bd4_slot28.p->field_01 = 0;
        func_80022c3c_slot28(obj, 3);
    }
}
