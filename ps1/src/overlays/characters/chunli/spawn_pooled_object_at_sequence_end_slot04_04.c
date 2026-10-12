/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_pooled_object_at_sequence_end_slot04_04(Object *obj) {
    Object *p;

    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x15;
            p->active_flag_2 = 1;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->scroll_factor = obj->scroll_factor;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->ground_y = obj->ground_y;
            p->parent = obj;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            play_side_sound_at(obj, obj->side, 0x11);
        }
        set_sequence_by_side_and_mode(obj, obj->direction_index + 0x5a);
    }
}
