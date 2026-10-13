/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../externs.h"

extern SequenceStep **seqs_b4_left;
extern SequenceStep **seqs_164_right;

void set_sequence_from_table_5(Object *object, int index, SequenceStep **table);

void set_variant_sequence_from_side_table_slot04_17(Object *obj, int idx);

void set_variant_sequence_from_side_table_slot04_17(Object *obj, int idx) {
    SequenceStep **table;

    if (obj->sound_side == 0) {
        table = seqs_b4_left;
    } else {
        table = seqs_164_right;
    }
    obj->variant_index = idx;
    set_sequence_from_table_5(obj, idx, table);
}
