/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2218_slot04_00(Object *obj) {
    Object *p;
    s16 t;
    s16 y;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x8000) {
        func_801312b8(obj);
    } else if ((t & 0xff00) == 0 && (t & 0xff) != 0) {
        obj->step_flags = 0;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0;
            p->table_index = 2;
            p->team_id = obj->team_id;
            p->sound_side = obj->sound_side;
            p->field_4b = obj->field_4b;
            p->vel_table_index = obj->direction_index + 6;
            p->health = (obj->direction_index >> 1) + 2;
            p->active_flag_2 = 1;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            play_side_sound_at(obj, obj->side, 0xb);
        }
    }
}
