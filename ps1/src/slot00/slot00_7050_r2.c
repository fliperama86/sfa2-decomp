/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80079878_slot00[];

void func_80077150_slot00(Object *obj) {
    Object *n = (Object *)func_8011f1e0();
    if (n != 0) {
        Object *p;
        n->active_count = 1;
        n->field_02 = 0xc;
        n->table_index = 8;
        n->field_0e = obj->field_0e;
        n->field_0c = obj->field_0c;
        n->image_row = obj->image_row;
        n->scroll_factor = obj->scroll_factor;
        n->parent = obj;
        p = obj->parent;
        p->offset_index = (p->offset_index + 2) & 0xe;
        n->facing_side = obj->facing_side;
        if (obj->facing_side == 0) {
            n->pos_x = obj->pos_x + data_80079878_slot00[p->offset_index];
        } else {
            n->pos_x = obj->pos_x - data_80079878_slot00[p->offset_index];
        }
        n->pos_y = obj->pos_y - data_80079878_slot00[p->offset_index + 1];
        n->mode_id = 0x20;
        n->field_90 = obj->field_90;
        n->data_ptr_a = obj->data_ptr_a;
        n->data_ptr_b = obj->data_ptr_b;
        n->spawn_x = obj->spawn_x;
        n->spawn_y = obj->spawn_y;
        n->sound_side = obj->sound_side;
    }
}
