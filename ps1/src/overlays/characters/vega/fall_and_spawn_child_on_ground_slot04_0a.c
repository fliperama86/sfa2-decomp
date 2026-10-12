/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void fall_and_spawn_child_on_ground_slot04_0a(Object *obj) {
    Object *c;

    *(u32 *)&obj->pos_y_frac = *(u32 *)&obj->pos_y_frac + 0xc000;
    if (obj->pos_y + 0x20 >= obj->ground_y) {
        obj->state_2 = obj->state_2 + 1;
        c = (Object *)pop_block_from_pool();
        if (c != 0) {
            c->active_count = c->active_count + 1;
            c->field_02 = 0xd;
            c->table_index = 0;
            c->parent = obj;
            c->spawn_x = obj->spawn_x;
            c->spawn_y = obj->spawn_y;
            c->image_row = obj->image_row;
            c->mode_id = 0x20;
            c->sound_side = obj->sound_side;
            c->field_90 = obj->field_90;
            c->data_ptr_a = obj->data_ptr_a;
            c->data_ptr_b = obj->data_ptr_b;
        }
    }
    update_sequence(obj);
}

void drop_to_ground_and_set_landing_sequence_slot04_0a(Object *obj) {
    *(u32 *)&obj->pos_y_frac = *(u32 *)&obj->pos_y_frac + 0xc000;
    if (obj->pos_y + 0x10 >= obj->ground_y) {
        obj->state_2 = obj->state_2 + 1;
        play_object_sound_by_kind(obj);
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        set_sequence_by_side(obj, 0x31);
    } else {
        update_sequence(obj);
    }
}
