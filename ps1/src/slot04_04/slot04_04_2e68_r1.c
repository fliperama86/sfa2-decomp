/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_two_objects_follow_linked_sequence_slot04_04(Object *obj) {
    Object *p;
    s16 t;

    p = (Object *)obj->linked_object;
    if (((s16)p->step_flags & 0x8000) == 0) {
        return;
    }
    t = obj->step_flags;
    if (t & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, obj->direction_index + 0x5b);
        return;
    }
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0;
            p->table_index = 1;
            p->ot_depth = 2;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->scroll_factor = obj->scroll_factor;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->ground_y = obj->ground_y;
            p->field_1c = obj->field_1c;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            p->mode_id = 0x20;
            p->sound_side = obj->sound_side;
            p->parent = obj;
            p = (Object *)pop_block_from_pool();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 0;
                p->table_index = 0;
                p->ot_depth = 0;
                p->field_0e = obj->field_0e;
                p->facing_side = obj->facing_side;
                p->scroll_factor = obj->scroll_factor;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                p->ground_y = obj->ground_y;
                p->field_1c = obj->field_1c;
                p->field_90 = (void *)0x800fb100;
                p->spawn_x = 0x60;
                p->spawn_y = 0x1e0;
                p->data_ptr_a = data_80172a48;
                p->data_ptr_b = data_80173c9c;
                p->mode_id = 0x20;
                p->sound_side = obj->sound_side;
                p->parent = obj;
            }
        }
    }
    obj->sequence = obj->sequence + 1;
    obj->step_timer = obj->sequence->duration;
    obj->step_flags = obj->sequence->flags;
    obj->frame = obj->frames + obj->sequence->frame_index;
}
