/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b1dc0_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p;
    u16 y;

    update_sequence(o);
    if (obj->mode != 0) {
        o->state_3++;
        o->countdown = obj->flag_bits | 0x400;
        p = ((Object *(*)(void))pop_object_from_pool)();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 2;
            p->active_flag_2 = 0;
            p->health = 0;
            p->table_index = 0;
            p->sound_side = o->sound_side;
            p->team_id = o->team_id;
            p->vel_table_index = o->direction_index >> 1;
            p->field_0e = o->field_0e;
            p->facing_side = o->facing_side;
            p->field_0c = o->field_0c;
            p->image_row = o->image_row;
            p->scroll_factor = o->scroll_factor;
            p->pos_x = o->pos_x;
            p->pos_y = o->pos_y;
            y = o->ground_y;
            p->spawn_x = 0x60;
            p->parent = o;
            p->spawn_y = 0x1e0;
            p->ground_y = y;
            p->field_90 = o->field_90;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
            o->linked_object_3 = (s32)p;
            o->link_flags++;
            play_side_sound_at(o, o->side, 0xb);
        }
    }
}
