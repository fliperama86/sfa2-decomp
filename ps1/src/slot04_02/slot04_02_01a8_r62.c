/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void unlink_from_parent_and_release_object_slot04_02(Object *o) {
    Object *p = o->parent;
    if (p->linked_object == (u32)o) {
        p->linked_object = 0;
    }
    clear_object_and_push_on_free_stack(o);
}

void set_sequence_from_variant_index_by_parent_side_slot04_02(Object *o) {
    if (o->parent->side == 0) {
        set_sequence_from_table_5(o, o->variant_index, data_1f8000b4);
    } else {
        set_sequence_from_table_5(o, o->variant_index, data_1f800164);
    }
}

void copy_parent_position_offset_by_facing_slot04_02(Object *o) {
    Object *p = o->parent;
    o->pos_x = p->pos_x;
    o->pos_y = p->pos_y;
    o->facing_side = 0;
    if (p->facing_side != 0) {
        o->pos_x = o->pos_x - 5;
    }
}
