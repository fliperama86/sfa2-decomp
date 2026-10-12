/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_linked_object_on_step_flag_type1_slot04_03(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 3;
            p->table_index = 1;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->ground_y = *(u8 *)&obj->step_flags;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 1;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->health = 0;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            p->parent = obj;
            obj->linked_object_3 = (s32)p;
            obj->countdown = obj->countdown + 0x100;
            obj->link_flags++;
        }
    }
}
