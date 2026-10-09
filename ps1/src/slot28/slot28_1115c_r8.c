/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051b9c_slot28;
extern SequenceStep *data_80045c6c_slot28[];
extern Object *data_80051b90_slot28[];
extern u8 data_80044620_slot28[];
extern u8 data_800449d0_slot28[];
extern Object *data_80051b64_slot28[];

void func_80021c84_slot28(Object *obj) {
    if ((s16)data_80051b9c_slot28.p->field_3a < 0) {
        data_8018f5a0->field_60 = 0xb4;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
    }
}

void func_80021cc4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t < 0) {
        func_8014f4d4(6, 3);
        func_801280f0();
        data_8018f5a0->field_52 += 1;
    }
}

void func_80021d2c_slot28(Object *obj) {
    if (game_state.field_f0 == 0) {
        data_8018f5a0->field_4e += 1;
    }
}

void func_80021d64_slot28(Object *obj, int arg) {
    func_80130768(data_80051b90_slot28[3], arg, data_80045c6c_slot28);
}

void func_80021d94_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_80044620_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_800449d0_slot28;
}

void func_80021dd0_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051b64_slot28[i] = 0;
    }
}
