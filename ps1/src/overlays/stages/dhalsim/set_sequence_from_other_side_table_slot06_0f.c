/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep **seqs_b4_left;
extern SequenceStep **seqs_164_right;

void set_sequence_from_other_side_table_slot06_0f(Object *obj, int idx) {
    SequenceStep **tbl;

    if (game_state.other_object->side == 0) {
        tbl = seqs_b4_left;
    } else {
        tbl = seqs_164_right;
    }
    set_sequence_from_table_5(obj, idx, tbl);
}
