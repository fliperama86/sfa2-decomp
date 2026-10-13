/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_800292cc_slot27[];
void set_sequence_from_other_kind_slot27(Object *obj);

void update_state_0_on_active_count_two_slot27(Object *obj) {
    set_sequence_from_other_kind_slot27(obj);
    if (obj->active_count == 2) {
        obj->state_0++;
    }
    if (ref_other.p->field_01 != 0) {
        if ((obj->variant_index & 0x80) == 0) {
            obj->field_01 = 1;
        }
    }
}

void set_sequence_from_other_kind_slot27(Object *obj) {
    ref_aux.p = ref_other.p->other;
    if (ref_aux.p->kind < 0x15) {
        set_sequence_from_table_5(obj, ref_aux.p->kind, data_800292cc_slot27[ref_aux.p->side]);
        obj->variant_index = ref_aux.p->kind;
        *(u16 *)&obj->pos_x = *(u16 *)&ref_other.p->pos_x;
        *(u16 *)&obj->pos_y = *(u16 *)&ref_other.p->pos_y;
    } else {
        obj->variant_index = 0xff;
    }
}

void increment_state_0_4_slot27(Object *obj) {
    obj->state_0++;
}
