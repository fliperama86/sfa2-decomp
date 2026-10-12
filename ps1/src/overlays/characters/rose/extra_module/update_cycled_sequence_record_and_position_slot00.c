/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void update_cycled_sequence_record_and_position_slot00(Object *obj) {
    Slot00Rec7ef0 *rec;
    s16 v;
    if (game_state.gate_flag != 0) goto fail;
    {
        v = (obj->cycle_value + 1) & 3;
        obj->cycle_value = v;
        if (v == 0) {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x40);
        } else if (v == 1) {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x50);
        } else if (v == 2) {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x60);
        } else {
            rec = (Slot00Rec7ef0 *)((u8 *)obj + 0x20);
        }
        obj->sequence = rec->seq;
        obj->step_timer = rec->step_timer;
        obj->step_flags = rec->step_flags;
        obj->sequence = obj->sequence + 1;
        if ((s16)rec->step_flags >= 0) goto ok;
    }
fail:
    obj->state_0 = 2;
    obj->field_01 = 0;
    return;
ok:
    {
            obj->step_timer = obj->sequence->duration;
            obj->step_flags = obj->sequence->flags;
            rec->seq = obj->sequence;
            rec->step_timer = obj->step_timer;
            rec->step_flags = obj->step_flags;
            obj->sequence_changed_flag = 1;
            rec->pos_x += rec->delta_x;
            rec->pos_y -= rec->delta_y;
            obj->pos_x = rec->pos_x;
            obj->pos_y = rec->pos_y;
            set_flag_and_push_if_mode_8(obj);
    }
}

void release_object_record_2_slot00(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
