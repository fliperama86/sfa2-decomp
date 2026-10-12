/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void start_sequence_4_when_variant_index_set_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        set_sequence_from_table_5(obj, 4, (SequenceStep **)obj->box_tables);
        obj->state_1 = obj->state_1 + 1;
    }
}
