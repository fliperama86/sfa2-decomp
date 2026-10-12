/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void shift_pos_x_by_parent_kind_table(Object *object) {
    int delta;
    ref_other.p = object->parent;
    delta = data_8017cf30[ref_other.p->kind];
    if (ref_other.p->facing_side != 0) {
        delta = -delta;
    }
    object->pos_x = delta + object->pos_x;
}

void call_block_handler_by_state_0(Block172 *block) {
    Object *object = (Object *)block;
    data_8017cf6c[object->state_0](object);
}
