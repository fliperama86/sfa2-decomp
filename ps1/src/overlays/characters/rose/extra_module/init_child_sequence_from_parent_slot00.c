/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void init_child_sequence_from_parent_slot00(Object *obj) {
    Object *t;
    obj->field_01 = 1;
    obj->state_0++;
    t = obj->parent;
    obj->pos_x = (u16)t->pos_x;
    obj->pos_y = (u16)t->pos_y;
    obj->frames = t->frames;
    if (obj->sound_side == 0) {
        obj->sequence = seqs_60_left[obj->variant_index];
    } else {
        obj->sequence = seqs_110_right[obj->variant_index];
    }
    obj->step_timer = obj->sequence->duration;
    obj->step_flags = obj->sequence->flags;
    obj->sequence_changed_flag = 1;
    obj->frame = obj->frames + obj->sequence->frame_index;
}

void slide_and_flicker_until_gate_flag_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    if (game_state.gate_flag == 0) {
        obj->pos_x_hi += o->vel_x;
        o->vel_x += o->accel_x;
        if (o->vel_x == 0) {
            o->vel_x = 0;
            o->accel_x = 0;
        }
        o->field_01 = 1;
        if (((game_state.frame_counter + o->table_index) & 1) == 0) {
            o->field_01 = 0;
        }
        advance_sequence_checked(o);
    } else {
        o->state_0 = 2;
    }
}
