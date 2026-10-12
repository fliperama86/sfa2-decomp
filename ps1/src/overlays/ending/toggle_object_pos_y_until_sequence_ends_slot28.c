/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051cd0_slot28[];
extern ObjectRef data_80051cdc_slot28;
extern Object *data_80051ca4_slot28[];

void toggle_object_pos_y_until_sequence_ends_slot28(Object *obj) {
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
    if ((s16)o->step_flags < 0) {
        data_8018f5a0->pass_index++;
        start_fade_mode_1a01_and_wait();
    }
}
