/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_800280bc_slot27[];

void move_down_and_follow_other_kind_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;
    Object *other = o->other;
    if (other->kind != o->sound_side) {
        o->sound_side = other->kind;
        set_sequence_from_table_5(o, other->kind, data_800280bc_slot27);
    }
    clear_or_set_flag_by_pos_x_limit(o);
    if (data_8018f5a0->substep_index >= 3 && game_state.mode != 3) {
        *(u16 *)&o->pos_y += 8;
        if (*(u16 *)&o->pos_y >= 0x2c1) {
            o->field_01 = 0;
            o->state_0++;
        }
    }
}

void increment_state_0_2_slot27(Object *obj) {
    obj->state_0++;
}

void release_object_to_pool_table_6_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
