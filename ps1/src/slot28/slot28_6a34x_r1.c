/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051cac_slot28;
extern ObjectRef data_80051cb0_slot28;
extern ObjectRef data_80051cdc_slot28;

void func_80026a34_slot28(Object *obj) {
    Object *p = data_80051cac_slot28.p;
    s16 x = p->pos_x;
    s16 y;
    if (x != 0x100) {
        p->pos_x = x - 1;
    }
    p = data_80051cb0_slot28.p;
    y = p->pos_x;
    if (y != 0x100) {
        p->pos_x = y - 1;
    }
    p = data_80051cdc_slot28.p;
    if ((s16)p->field_3a < 0) {
        p = (Object *)data_8018f5a0;
        ((HudState *)p)->field_52++;
        func_80128370();
    }
}
