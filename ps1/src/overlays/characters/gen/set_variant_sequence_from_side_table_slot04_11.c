/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void set_sequence_from_table_5(Object *object, int index, SequenceStep **table);

/* set_sequence_from_table_5 is declared here with an int second parameter, not the s16
   of protos.h, which is why this unit does not include protos.h. */
void set_variant_sequence_from_side_table_slot04_11(Object *obj, int idx) {
    SequenceStep **table;

    if (obj->sound_side == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    obj->variant_index = idx;
    set_sequence_from_table_5(obj, idx, table);
}
