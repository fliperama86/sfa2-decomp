/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800519d8_slot28[];
extern ObjectRef data_80051a10_slot28;
void func_8001c2b4_slot28(Object *o);

void func_8001c0d4_slot28(Object *o) {
    Object *p = data_80051a10_slot28.p;
    if ((s16)p->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_60 = 0xf0;
        h->field_52++;
        p = data_800519d8_slot28[0];
        p->field_48 = 0xff;
        func_8001c2b4_slot28(o);
    }
}
