/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2158_slot04_0a(Object *obj) {
    Object *n;

    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        play_side_sound_at(obj, obj->side, 0xa);
        play_side_sound_at(obj, obj->side, 0xe);
        n = pop_object_from_pool(obj);
        if (n != 0) {
            n->active_count = 1;
            n->field_02 = 0xa;
            n->sound_side = obj->sound_side;
            n->team_id = obj->team_id;
            n->vel_table_index = obj->direction_index;
            n->active_flag_2 = 0;
            n->field_0e = obj->field_0e;
            n->facing_side = obj->facing_side;
            n->field_0c = obj->field_0c;
            n->image_row = obj->image_row;
            n->scroll_factor = obj->scroll_factor;
            *(u16 *)&n->pos_x = *(u16 *)&obj->pos_x;
            *(u16 *)&n->pos_y = *(u16 *)&obj->pos_y;
            n->spawn_x = 0x60;
            n->health = 0;
            n->parent = obj;
            n->spawn_y = 0x1e0;
            n->field_90 = obj->field_90;
            n->data_ptr_a = obj->data_ptr_a;
            n->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)n;
            obj->link_flags++;
        }
    }
    update_sequence(obj);
}
