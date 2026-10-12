/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_four_if_variant_set_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1 = obj->state_1 + 1;
        set_sequence_from_table_5(obj, 4, (SequenceStep **)obj->box_tables);
    }
}
