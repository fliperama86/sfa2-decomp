/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_linked_pooled_object_on_step_flag_2_slot04_0b(Object *obj) {
    Object *p;
    u16 t;
    s16 y;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) == 2) {
        obj->step_flags = t & 0xff00;
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xb;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->field_4b = obj->field_4b;
            p->vel_table_index = obj->direction_index + 0xc;
            p->active_flag_2 = 1;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->countdown_2 = 0;
            p->parent = obj;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
        }
    }
}

void reset_object_on_high_flag_byte_else_update_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
