/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void set_sequence_1_by_parent_side_slot00(Object *o) {
    SequenceStep **t;

    o->state_1++;
    if (o->parent->side == 0) {
        t = data_1f8000ac;
    } else {
        t = data_1f80015c;
    }
    set_sequence_from_table_5(o, 1, t);
}

void advance_sequence_or_set_state_3_slot00(Object *o) {
    if ((s16)o->step_flags >= 0) {
        advance_sequence_checked(o);
    } else {
        o->state_0 = 3;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
    }
}

void unlink_from_parent_and_release_slot00(Object *o) {
    ref_other.p = o->parent;
    ref_other.p->link_flags--;
    if (ref_other.p->link_flags == 0) {
        ref_other.p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}
