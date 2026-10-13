/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_variant_sequence_by_sound_side_slot04_06(Object *obj, int idx) {
    SequenceStep **table;

    obj->variant_index = idx;
    if (obj->sound_side == 0) {
        table = seqs_b4_left;
    } else {
        table = seqs_164_right;
    }
    set_sequence_from_table_5(obj, (u8)idx, table);
}
