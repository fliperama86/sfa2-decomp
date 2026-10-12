/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void add_delta_to_parent_if_counter_zero_slot00(Object *obj, short delta);

void spawn_effect_and_raise_parent_variant_slot00(Object *obj) {
    Object *p = obj->parent;
    Object *n;
    Object *q;
    if (p->variant_index_2 != 7) {
        p->variant_index_2 = p->variant_index_2 + 1;
    }
    obj->state_0 = obj->state_0 + 1;
    add_delta_to_parent_if_counter_zero_slot00(obj, 0x12);
    q = ((Slot00Obj *)obj)->field_a8;
    n = (Object *)pop_block_from_pool();
    if (n != 0) {
        n->active_count = 1;
        n->table_index = 6;
        ((Slot00Obj *)n)->pos_x_hi = ((Slot00Obj *)q)->pos_x_hi;
        ((Slot00Obj *)n)->pos_y_hi = ((Slot00Obj *)q)->pos_y_hi;
        n->facing_side = p->facing_side;
        n->field_0e = p->field_0e;
        n->field_0c = p->field_0c;
        n->image_row = p->image_row;
        n->field_1c = p->field_1c;
        n->cycle_value = p->cycle_value;
        n->field_02 = 0xc;
        n->mode_id = 0x20;
        n->field_90 = p->field_90;
        n->data_ptr_a = p->data_ptr_a;
        n->data_ptr_b = p->data_ptr_b;
        n->spawn_x = p->spawn_x;
        n->spawn_y = p->spawn_y;
        n->sound_side = p->side;
    }
    play_side_sound_at(p, p->side, 0x15);
}
