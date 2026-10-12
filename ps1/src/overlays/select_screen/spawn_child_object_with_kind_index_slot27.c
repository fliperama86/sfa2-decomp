/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void spawn_child_object_with_kind_index_slot27(Object *obj) {
    Object *p = (Object *)pop_block_from_pool();
    Object *b = p;
    if (b != 0) {
        b->active_count = 1;
        b->field_02 = 0xaa;
        b->table_index = game_state.kind_index;
        b->ot_depth = 8;
        b->field_90 = (void *)0x80038000;
        b->data_ptr_a = data_80017c28_slot27;
        b->data_ptr_b = data_8001aa14_slot27;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        obj->parent = b;
    }
}
