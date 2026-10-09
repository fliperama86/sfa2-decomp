/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051b10_slot28;
extern SequenceStep *data_800414c0_slot28[];
extern Object *data_80051b04_slot28[];
extern u8 data_8003f9fc_slot28[];
extern u8 data_8003feb8_slot28[];
extern Object *data_80051ad8_slot28[];

void func_8001ff7c_slot28(Object *obj) {
    if ((s16)data_80051b10_slot28.p->field_3a < 0) {
        data_8018f5a0->field_60 = 0xb4;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
    }
}

void func_8001ffbc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t < 0) {
        func_8014f4d4(6, 3);
        func_801280f0();
        data_8018f5a0->field_52 += 1;
    }
}

void func_80020024_slot28(Object *obj) {
    if (game_state.field_f0 == 0) {
        data_8018f5a0->field_4e += 1;
    }
}

void func_8002005c_slot28(Object *obj, int arg) {
    func_80130768(data_80051b04_slot28[3], arg, data_800414c0_slot28);
}

void func_8002008c_slot28(Object *obj) {
    int one = 1;
    obj->field_00 = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_8003f9fc_slot28;
    obj->field_03 = 0;
    obj->field_01 = one;
    obj->field_9c = data_8003feb8_slot28;
}

void func_800200c8_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051ad8_slot28[i] = 0;
    }
}
