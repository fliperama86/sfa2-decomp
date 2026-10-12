/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep *data_801c59b0_slot04_02[];

void set_sequence_from_variant_index_slot04_02(Object *obj) {
    set_sequence_from_table_5(obj, (u8)(obj->variant_index - 1), data_801c59b0_slot04_02);
}

void copy_parent_position_with_facing_offset_slot04_02(Object *obj) {
    Object *p = obj->parent;

    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->facing_side = 0;
    if (p->facing_side != 0) {
        obj->pos_x = obj->pos_x - 0x18;
    }
}
