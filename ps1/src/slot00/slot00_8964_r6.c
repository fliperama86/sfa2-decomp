/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a12c_slot00[];

void func_80078e44_slot00(Object *o) {
    Object *obj = o;
    Object *p = (Object *)func_8011f1e0();
    Object *src;
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0xc;
        p->mode_id = 0x20;
        p->table_index = 0xa;
        p->field_0e = obj->field_0e;
        src = obj->parent;
        p->field_0c = src->field_0c;
        p->image_row = src->image_row;
        p->scroll_factor = obj->scroll_factor;
        p->parent = o;
        p->facing_side = obj->facing_side;
        p->pos_x = (u16)o->pos_x;
        p->pos_y = (u16)o->pos_y;
        if (player_left.kind == 9) {
            src = &player_left;
        } else {
            src = &player_left + 1;
        }
        p->field_90 = src->field_90;
        p->data_ptr_a = src->data_ptr_a;
        p->data_ptr_b = src->data_ptr_b;
        p->spawn_x = src->spawn_x;
        p->spawn_y = src->spawn_y;
        p->sound_side = src->side;
    }
}

void func_80078f50_slot00(Object *obj) {
    data_8007a12c_slot00[obj->state_0](obj);
}
