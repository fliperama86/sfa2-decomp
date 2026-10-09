/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_80051a10_slot28;
extern Object *data_800519d8_slot28[];
extern u16 data_80051eb8_slot28[];

void func_80128410(void);

void func_8001b660_slot28(Object *obj) {
    Object *p = data_80051a10_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        HudState *h = data_8018f5a0;
        h->field_52 = h->field_52 + 1;
        p = data_800519d8_slot28[2];
        p->field_04 = 2;
        data_80051eb8_slot28[0] = 0xfff;
        func_80128410();
    }
}
