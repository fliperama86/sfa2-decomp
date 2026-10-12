/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051cd0_slot28[];
extern ObjectRef data_80051cdc_slot28;

void bounce_object_y_until_step_flag_slot28(Object *obj) {
    Object *o = data_80051cd0_slot28[0];
    if ((u32)(o->pos_y - 0x10) >= 9) {
        o->vel_y = -o->vel_y;
    }
    *(s32 *)&o->pos_y_frac = *(s32 *)&o->pos_y_frac + o->vel_y;
    o = data_80051cdc_slot28.p;
    if ((s16)o->step_flags < 0) {
        data_8018f5a0->countdown_timer = 0xb4;
        data_8018f5a0->pass_index++;
    }
}
