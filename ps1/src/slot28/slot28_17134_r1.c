/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80051cd0_slot28[];
extern ObjectRef data_80051cdc_slot28;

void func_80027134_slot28(Object *obj) {
    Object *o = data_80051cd0_slot28[0];
    if ((u32)(o->pos_y - 0x10) >= 9) {
        o->field_50 = -o->field_50;
    }
    *(s32 *)&o->field_14 = *(s32 *)&o->field_14 + o->field_50;
    o = data_80051cdc_slot28.p;
    if ((s16)o->field_3a < 0) {
        data_8018f5a0->field_60 = 0xb4;
        data_8018f5a0->field_52++;
    }
}
