/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_800519d8_slot28[];
extern Object *data_80051a04_slot28[];
extern ObjectRef data_80051a08_slot28;
void func_8001c430_slot28(Object *obj, int arg);

void func_8001b9d0_slot28(Object *obj) {
    if (obj->field_f0 != 0) {
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        func_8001c430_slot28(obj, 1);
    }
}

void func_8001ba18_slot28(Object *obj) {
    Object *o;
    int i;
    Object **q;
    i = 0;
    q = data_800519d8_slot28;
    while (i < 6) {
        o = *q++;
        ((Slot28Obj *)o)->field_14 -= 0x4000;
        i++;
    }
    o = data_80051a08_slot28.p;
    ((Slot28Obj *)o)->field_14 -= 0x4000;
    o = data_80051a04_slot28[0];
    ((Slot28Obj *)o)->field_14 -= 0x4000;
    if (o->pos_y < -0x6f) {
        data_8018f5a0->field_60 = 0x258;
        data_8018f5a0->field_52 = data_8018f5a0->field_52 + 1;
        o->pos_y = -0x70;
        func_8001c430_slot28(obj, 2);
    }
}
