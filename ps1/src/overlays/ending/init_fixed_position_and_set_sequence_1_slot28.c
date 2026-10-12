/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_fixed_position_and_set_sequence_1_slot28(Object *obj) {
    obj->pos_x = 0xb8;
    obj->pos_y = 0x100;
    obj->field_0c = 0;
    obj->field_0e = 0;
    obj->facing_side = 0;
    obj->state_0++;
    set_sequence_from_table_5(obj, 1, (SequenceStep **)obj->box_tables);
}
