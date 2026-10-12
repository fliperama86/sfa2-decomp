/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5ab4_slot04_09(Object *o) {
    Object *obj = o;
    Object *p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0xc;
        p->mode_id = 0x20;
        p->table_index = 0x14;
        p->field_0e = obj->field_0e;
        p->field_0c = obj->field_0c;
        p->image_row = obj->image_row;
        p->scroll_factor = obj->scroll_factor;
        p->parent = o;
        p->pos_x = (u16)o->pos_x;
        p->pos_y = (u16)o->pos_y;
        p->facing_side = obj->facing_side;
        p->field_90 = obj->field_90;
        p->data_ptr_a = obj->data_ptr_a;
        p->data_ptr_b = obj->data_ptr_b;
        p->spawn_x = obj->spawn_x;
        p->spawn_y = obj->spawn_y;
        p->sound_side = obj->side;
    }
}
