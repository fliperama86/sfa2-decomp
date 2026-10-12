/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 apply_table_step_to_pos_x(Object *o) {
    u8 ret = 1;
    int d;
    o->step_applied_flag = 0;
    if ((o->countdown & 1) && o->bound_push_flag == 0 && o->active_state == 0) spawn_random_offset_object_variant_zero(o);
    d = table_80170244[o->field_62][(s16)o->countdown];
    if ((d & 0x80) == 0) {
        o->countdown = (s16)o->countdown + 1;
        ret = 0;
        o->step_applied_flag = 1;
        if (o->facing_flag == 0) d = -d;
        if ((s16)o->health < 0) d <<= 1;
        o->pos_x = o->pos_x + d;
    }
    return ret;
}

void move_pos_x_by_countdown_parity(Object *o) {
    int d, x;
    call_handler_by_mode_flag_2(o);
    o->step_applied_flag = 0;
    if (o->countdown & 1) {
        if (o->bound_push_flag == 0 && o->active_state == 0) spawn_random_offset_object_variant_zero(o);
    } else {
        d = 1;
        o->step_applied_flag = 1;
        if (o->facing_flag == 0) d = -1;
        if ((s16)o->health < 0) d <<= 1;
        x = d + (u16)o->pos_x;
        o->pos_x = x;
        d = 3;
        if (o->facing_side == 0) d = -3;
        if (o->field_6b & 1) d = -d;
        o->pos_x = d + x;
    }
}
