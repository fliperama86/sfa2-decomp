/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051ba4_slot28[];
extern ObjectRef data_80051bdc_slot28;
void func_80022c3c_slot28(Object *obj, int arg);

void func_80022594_slot28(Object *obj) {
    Object *a;
    a = data_80051bdc_slot28.p;
    if (((Slot28Obj *)a)->field_3a < 0) {
        data_8018f5a0->field_60 = 0x1a4;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        a = data_80051ba4_slot28[0];
        a->field_48 = 0xff;
        a = data_80051ba4_slot28[1];
        a->field_48 = 0xff;
        func_8014f4d4(6, 3);
        data_8018f5a0->field_60 = 0x1e;
    }
}

void func_80022618_slot28(Object *obj) {
    int t = data_8018f5a0->field_60 - 1;
    data_8018f5a0->field_60 = t;
    if ((s16)t < 0) {
        data_8018f5a0->field_60 = 0x1a4;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_80022c3c_slot28(obj, 1);
    }
}

void func_8002266c_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051bdc_slot28.p)->field_3a < 0) {
        func_8014f4d4(1, 0x505);
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_801282d4();
    }
}
