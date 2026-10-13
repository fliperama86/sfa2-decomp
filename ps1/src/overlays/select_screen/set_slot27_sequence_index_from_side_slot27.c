/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"
extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_8002904c_slot27[];
extern ObjectFn data_80028e84_slot27[];
void offset_position_by_selection_slot27(Object *obj);

void set_slot27_sequence_index_from_side_slot27(Object *unused) {
    int t = (*(Object **)&ref_aux)->side * 2;
    scratch_word_a[0] = t;
    scratch_word_a[0] = ref_other.p->table_index + t;
}

void offset_position_then_clear_offset_byte_slot27(Object *obj) {
    offset_position_by_selection_slot27(obj);
    scratch_word_a[0] = 0;
}
