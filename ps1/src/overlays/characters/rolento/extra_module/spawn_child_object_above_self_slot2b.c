/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void spawn_child_object_above_self_slot2b(Object *o) {
    Object *p = o->parent;
    Object *n = (Object *)pop_block_from_pool();
    if (n != 0) {
        n->active_count = 1;
        n->field_02 = 0x3b;
        n->table_index = 1;
        n->field_02 = 0x14;
        n->ot_depth = 0;
        n->pos_x = (u16)o->pos_x;
        n->pos_y = (u16)o->pos_y - 0x10;
        n->variant_index = 0x51;
        n->team_id = o->team_id;
        n->spawn_x = 0x60;
        n->parent = p;
        n->spawn_flag = 1;
        n->spawn_y = 0x1e0;
        n->image_row = o->image_row;
        n->mode_id = 0x20;
        n->field_90 = o->field_90;
        n->sound_side = o->sound_side;
        n->data_ptr_a = o->data_ptr_a;
        n->data_ptr_b = o->data_ptr_b;
        play_side_sound_at(o, p->side, 0xf);
    }
}
