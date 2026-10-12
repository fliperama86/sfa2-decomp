/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_fourth_frame_child_index_zero_slot04_09(Object *o) {
    Object *p;
    u16 y;
    if ((game_state.frame_counter & 3) == 0) {
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xb;
            p->mode_id = 0x20;
            p->table_index = 0;
            p->field_0e = o->field_0e;
            p->parent = o;
            p->facing_side = o->facing_side;
            p->pos_x = (u16)o->pos_x;
            y = (u16)o->pos_y;
            p->spawn_flag = 1;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->data_ptr_a = &data_80172a48;
            p->field_90 = (void *)0x800fb100;
            p->image_row = 0;
            p->data_ptr_b = &data_80173c9c;
            p->pos_y = y;
            p->sound_side = o->side;
        }
    }
}

void spawn_fourth_frame_child_index_four_slot04_09(Object *o) {
    Object *p;
    u16 y;
    if ((game_state.frame_counter & 3) == 0) {
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xb;
            p->mode_id = 0x20;
            p->table_index = 4;
            p->field_0e = o->field_0e;
            p->parent = o;
            p->facing_side = o->facing_side;
            p->pos_x = (u16)o->pos_x;
            y = (u16)o->pos_y;
            p->spawn_flag = 1;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->data_ptr_a = &data_80172a48;
            p->field_90 = (void *)0x800fb100;
            p->image_row = 0;
            p->data_ptr_b = &data_80173c9c;
            p->pos_y = y;
            p->sound_side = o->side;
        }
    }
}
