/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_variant_sequence_by_sound_side_slot04_06(Object *obj, int idx) {
    SequenceStep **table;

    obj->variant_index = idx;
    if (obj->sound_side == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    set_sequence_from_table_5(obj, (u8)idx, table);
}
