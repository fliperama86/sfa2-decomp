/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void start_sequence_9_when_variant_set_slot28(Object *o) {
    if (o->variant_index != 0) {
        o->variant_index = 0;
        o->state_1++;
        set_sequence_from_table_5(o, 9, ((Slot28Obj *)o)->sequence_ptr);
    }
}

void advance_sequence_start_sequence_5_on_variant_slot28(Object *o) {
    advance_sequence_checked(o);
    if (o->variant_index != 0) {
        o->variant_index = 0;
        o->ot_depth = 4;
        o->state_1++;
        set_sequence_from_table_5(o, 5, ((Slot28Obj *)o)->sequence_ptr);
    } else {
        int v = ((Slot28Obj *)o)->field_3a;
        o->ot_depth = (v == 1) ? 4 : 2;
    }
}

void start_sequence_10_unless_variant_ff_slot28(Object *o) {
    if (o->variant_index != 0xff) {
        o->variant_index = 0;
        o->state_1++;
        set_sequence_from_table_5(o, 10, ((Slot28Obj *)o)->sequence_ptr);
    }
}
