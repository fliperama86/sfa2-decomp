/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_800797fc_slot00[];
extern ObjectFn data_8007980c_slot00[];

void func_80076e54_slot00(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown < 0) {
        o->active_count = 1;
        o->state_0 = 1;
        o->state_2 = 1;
        o->state_1 = 0;
        o->state_3 = 0;
        o->vel_x = -o->vel_x;
        if (o->active_flag_2 != 0) {
            o->vel_y = data_800797fc_slot00[3];
        } else {
            o->vel_y = data_800797fc_slot00[o->vel_table_index >> 1];
        }
    }
    advance_sequence_checked(o);
}

void func_80076ef0_slot00(Object *o) {
    data_8007980c_slot00[o->state_1](o);
    func_8011ffdc(o);
}
