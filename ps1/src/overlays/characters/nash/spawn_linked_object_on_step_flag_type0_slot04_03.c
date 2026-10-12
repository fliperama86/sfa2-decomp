/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_linked_object_on_step_flag_type0_slot04_03(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 3;
            p->table_index = 0;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->ground_y = 0;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
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
            obj->link_flags++;
        }
    }
}

void update_sequence_or_reset_on_end_flag_slot04_03(Object *obj) {
    s16 t = obj->step_flags;

    if (t == 0x100) {
        obj->step_flags = (u8)t;
        obj->active_flag_4 = 0;
    }
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}
