/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b57b4_slot04_09(Object *obj) {
    Object *n;

    n = (Object *)func_8011f1e0();
    if (n != 0) {
        n->active_count = 1;
        n->field_02 = 0xc;
        n->mode_id = 0x20;
        n->table_index = 4;
        n->field_0e = obj->field_0e;
        n->field_0c = obj->field_0c;
        n->image_row = obj->image_row;
        n->scroll_factor = obj->scroll_factor;
        n->parent = obj;
        n->facing_side = obj->facing_side;
        n->pos_x = obj->pos_x;
        n->pos_y = obj->pos_y;
        if (obj->facing_side != 0) {
            n->pos_x = n->pos_x + 0x31;
        } else {
            n->pos_x = n->pos_x - 0x31;
        }
        n->pos_y = n->pos_y - 0x52;
        n->field_90 = obj->field_90;
        n->data_ptr_a = obj->data_ptr_a;
        n->data_ptr_b = obj->data_ptr_b;
        n->spawn_x = obj->spawn_x;
        n->spawn_y = obj->spawn_y;
        n->sound_side = obj->side;
    }
}
