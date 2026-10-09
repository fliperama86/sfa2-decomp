/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_8005198c_slot28;
extern ObjectRef data_80051958_slot28;
void func_80019090_slot28(Object *obj);
void func_80019100_slot28(Object *obj, int arg);

void func_80018cec_slot28(Object *obj) {
    Object *p;
    func_80019090_slot28(obj);
    p = data_8005198c_slot28.p;
    if ((s16)p->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_60 = 0xb4;
        h->field_52 += 1;
        p = data_80051958_slot28.p;
        p->field_48 = 0xff;
        func_80019100_slot28(obj, 5);
    }
}
