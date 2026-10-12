/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_800797fc_slot00[];
extern ObjectFn data_8007980c_slot00[];

void reverse_vel_x_set_vel_y_on_countdown_slot00(Object *o) {
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

void call_state_1_table_handler_then_cull_check_slot00(Object *o) {
    data_8007980c_slot00[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}
