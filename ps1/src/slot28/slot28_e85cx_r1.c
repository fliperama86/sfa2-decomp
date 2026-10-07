/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051a98_slot28[];
extern ObjectRef data_80051ad0_slot28;

void func_8001e85c_slot28(Object *obj) {
    Object *p = data_80051ad0_slot28.p;
    if ((s16)p->field_3a < 0) {
        data_8018f5a0->field_60 = 0xb4;
        data_8018f5a0->field_52++;
        p = data_80051a98_slot28[0];
        p->field_48 = 0xff;
        p = data_80051a98_slot28[1];
        p->field_48 = 0xff;
    }
}
