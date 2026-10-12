/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void randomize_position_velocity_and_sequence_slot28(Object *obj);

void init_object_sequence_by_table_index_slot28(Object *obj) {
    int kind = obj->table_index;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0 = obj->state_0 + 1;
    if (kind != 2) {
        if (kind == 3) {
            randomize_position_velocity_and_sequence_slot28(obj);
        } else {
            set_sequence_from_table_5(obj, kind, (SequenceStep **)obj->box_tables);
        }
    } else {
        obj->ot_depth = 1;
        set_sequence_from_table_5(obj, 6, (SequenceStep **)obj->box_tables);
    }
}
