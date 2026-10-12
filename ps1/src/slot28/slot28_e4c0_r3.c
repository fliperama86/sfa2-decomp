/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ac4_slot28[];
extern ObjectRef data_80051ac8_slot28;
extern ObjectRef data_80051ad0_slot28;
void func_8001ef04_slot28(void);
void func_8001ee74_slot28(Object *obj, int arg);

void func_8001e90c_slot28(Object *obj) {
    if (obj->field_f0 == 0) {
        data_8018f5a0->pass_index++;
        func_8001ef04_slot28();
        data_80051ac4_slot28[0]->field_01 = 0;
        data_80051ac8_slot28.p->field_01 = 0;
        func_8001ee74_slot28(obj, 1);
        func_80128370();
    }
}

void func_8001e98c_slot28(Object *obj) {
    if ((s16)data_80051ad0_slot28.p->step_flags < 0) {
        data_8018f5a0->countdown_timer = 0xb4;
        data_8018f5a0->pass_index++;
        func_8014f4d4(6, 1);
    }
}

void func_8001e9e4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        h->pass_index++;
        func_8014f4d4(1, 0x507);
        func_801282d4();
    }
}
