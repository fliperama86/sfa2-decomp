/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_800518d0_slot28[];
extern ObjectRef data_80051908_slot28;
extern SequenceStep *data_8002fca8_slot28[];
void func_80016e28_slot28(Object *obj, int arg);

void func_80016cc0_slot28(Object *o) {
    Object *p = data_80051908_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        p = (Object *)data_8018f5a0;
        ((HudState *)p)->field_60 = 0x12c;
        ((HudState *)p)->field_52++;
        p = data_800518d0_slot28[0];
        p->field_48 = 0xff;
        if ((((Slot28Obj *)o)->field_70 & 0x7f) == 4) {
            func_80130768(p, 0xc, data_8002fca8_slot28);
        } else {
            func_80130768(p, 0xe, data_8002fca8_slot28);
        }
        p = data_800518d0_slot28[1];
        p->field_48 = 0xff;
        if ((((Slot28Obj *)o)->field_70 & 0x7f) == 4) {
            func_80130768(p, 0xd, data_8002fca8_slot28);
        } else {
            func_80130768(p, 0xf, data_8002fca8_slot28);
        }
        func_80016e28_slot28(o, 5);
    }
}

void func_80016d94_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051908_slot28.p)->field_3a < 0) {
        func_8014f4d4(6, 3);
        func_801280f0();
        data_8018f5a0->field_52++;
    }
}
