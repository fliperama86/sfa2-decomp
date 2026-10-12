/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_child_object_at_parent_offset_slot04_01(Object *obj, int kind) {
    Object *p;
    int a = -0x18;
    u8 t;

    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 1;
        p->table_index = game_state.frame_counter;
        p->field_0e = obj->field_0e;
        p->ot_depth = 4;
        p->variant_index = kind;
        p->parent = obj;
        p->field_0c = obj->field_0c;
        p->field_1c = obj->field_1c;
        p->spawn_x = obj->spawn_x;
        p->spawn_y = obj->spawn_y;
        p->image_row = obj->image_row;
        p->mode_id = 0x20;
        p->sound_side = obj->sound_side;
        p->field_90 = obj->field_90;
        p->data_ptr_a = obj->data_ptr_a;
        p->data_ptr_b = obj->data_ptr_b;
        if (kind != 7) {
            a = 0x14;
        }
        t = obj->facing_side;
        p->facing_side = t;
        if (t != 0) {
            p->pos_x = a + p->pos_x;
        } else {
            p->pos_x = p->pos_x - a;
        }
        p->pos_y = obj->pos_y - 0x74;
    }
}
