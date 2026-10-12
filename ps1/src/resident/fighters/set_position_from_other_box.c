/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_position_from_other_box(Object *object) {
    Box6 *box;
    u16 x;
    u16 ox;
    u8 i;
    ref_other.p = object->other;
    if (object->side == 0) {
        box = boxes_74_left[ref_other.p->kind];
    } else {
        box = boxes_124_right[ref_other.p->kind];
    }
    i = object->frame->box_table_index;
    x = ref_other.p->pos_x;
    box += i;
    ox = box->origin;
    if (object->facing_side != 0) ox = -ox;
    object->pos_x = x - ox;
    x = ref_other.p->pos_y;
    object->pos_y = x + box->origin_y;
}
