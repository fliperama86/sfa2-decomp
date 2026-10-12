/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 5 instruction slots. */
void update_sequence_spawn_child_or_finish_slot04_02(Object *obj) {
    Object *p;
    s16 t;
    u16 y;

    update_sequence(obj);
    t = obj->step_flags;
    if (!(t & 0x8000)) {
        if ((t & 0xff) != 0) {
            obj->step_flags = t & 0xff00;
            p = ((Object *(*)(void))pop_object_from_pool)();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 2;
                p->table_index = 6;
                p->sound_side = obj->sound_side;
                p->team_id = obj->team_id;
                p->field_4b = obj->field_4b;
                p->vel_table_index = (obj->direction_index >> 1) + 9;
                p->health = obj->direction_index + 3;
                p->active_flag_2 = 1;
                p->facing_side = obj->facing_side;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->image_row = obj->image_row;
                p->scroll_factor = obj->scroll_factor;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                y = obj->ground_y;
                p->spawn_x = 0x60;
                p->parent = obj;
                p->spawn_y = 0x1e0;
                p->ground_y = y;
                p->field_90 = obj->field_90;
                p->data_ptr_a = obj->data_ptr_a;
                p->data_ptr_b = obj->data_ptr_b;
                obj->linked_object_3 = (s32)p;
                obj->link_flags++;
                play_side_sound_at(obj, obj->side, 0xb);
            }
        }
    } else {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
