/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80128370(void);

extern HudState *data_8018f5a0;
extern Object *data_80051cd0_slot28[];
extern ObjectRef data_80051cdc_slot28;
extern Object *data_80051ca4_slot28[];

void func_80026d54_slot28(Object *obj) {
    Object *o;
    o = data_80051cd0_slot28[0];
    if (o->pos_y == 0x10) {
        o->pos_y = 0x18;
    } else {
        o->pos_y = 0x10;
    }
    o = data_80051ca4_slot28[4];
    if (o->pos_y == 0xa0) {
        o->pos_y = 0xa8;
    } else {
        o->pos_y = 0xa0;
    }
    o = data_80051ca4_slot28[5];
    if (o->pos_y == 0xa0) {
        o->pos_y = 0xa8;
    } else {
        o->pos_y = 0xa0;
    }
    o = data_80051cdc_slot28.p;
    if ((s16)o->field_3a < 0) {
        data_8018f5a0->field_52++;
        func_80128370();
    }
}
