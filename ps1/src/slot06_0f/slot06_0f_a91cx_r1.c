/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void set_sequence_from_other_side_table_slot06_0f(Object *obj, int idx) {
    SequenceStep **tbl;

    if (game_state.other_object->side == 0) {
        tbl = data_1f8000b4;
    } else {
        tbl = data_1f800164;
    }
    set_sequence_from_table_5(obj, idx, tbl);
}
