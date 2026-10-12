/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void randomize_position_velocity_and_sequence_2_slot28(Object *obj);

void init_object_sequence_by_table_index_2_slot28(Object *obj) {
    int t = obj->table_index;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->state_0 = obj->state_0 + 1;
    switch (t) {
    case 3:
        randomize_position_velocity_and_sequence_2_slot28(obj);
        break;
    default:
        set_sequence_from_table_5(obj, t != 0 ? 3 : 1, (SequenceStep **)obj->box_tables);
        break;
    case 2:
        obj->ot_depth = 1;
        set_sequence_from_table_5(obj, 3, (SequenceStep **)obj->box_tables);
        break;
    }
}
