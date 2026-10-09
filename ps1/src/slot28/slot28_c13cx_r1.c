/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800519d8_slot28[];
void func_8001c3ac_slot28(Object *o);
void func_8001c430_slot28(Object *o, int n);

void func_8001c13c_slot28(Object *o) {
    HudState *h = data_8018f5a0;
    Object *p;
    int t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t == 0) {
        h->field_60 = 0x258;
        h->field_52++;
        p = data_800519d8_slot28[0];
        p->field_48 = 0xff;
        p = data_800519d8_slot28[1];
        p->field_48 = 0xff;
        func_8001c3ac_slot28(o);
        func_80120554(0, 0, 0x305);
        func_8001c430_slot28(o, 5);
    }
}
