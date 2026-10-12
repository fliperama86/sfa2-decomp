/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void start_sequence_2_on_variant_index_slot28(Object *o) {
    if (o->variant_index != 0) {
        o->state_1++;
        o->variant_index = 0;
        set_sequence_from_table_5(o, 2, (SequenceStep **)o->box_tables);
    }
}

void start_sequence_3_on_variant_index_slot28(Object *o) {
    if (o->variant_index != 0) {
        o->state_1++;
        o->variant_index = 0;
        o->countdown = 0;
        set_sequence_from_table_5(o, 3, (SequenceStep **)o->box_tables);
    }
}

void jitter_pos_y_by_timer_reload_phase_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    int t = obj->timer_reload + 1;
    int d = 0x10000;
    obj->timer_reload = t & 3;
    if (t & 2) {
        d = -0x10000;
    }
    obj->pos_y = obj->pos_y + d;
}
