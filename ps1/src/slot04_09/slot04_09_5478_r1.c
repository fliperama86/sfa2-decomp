/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_linked_pooled_object_health_2_slot04_09(Object *obj) {
    Object *n = pop_object_from_pool(obj);

    if (n != 0) {
        n->active_count = 1;
        n->field_02 = 9;
        n->table_index = 0;
        n->sound_side = obj->sound_side;
        n->team_id = obj->team_id;
        n->field_49 = obj->field_49;
        n->vel_table_index = obj->direction_index;
        n->table_index_3 = 0;
        n->active_flag_2 = 1;
        n->field_4b = obj->field_4b;
        n->field_0e = obj->field_0e;
        n->facing_side = obj->facing_side;
        n->field_0c = obj->field_0c;
        n->image_row = obj->image_row;
        n->scroll_factor = obj->scroll_factor;
        n->health = 2;
        *(u32 *)&n->field_a4 = 0x1000800;
        n->parent = obj;
        obj->linked_object_3 = (s32)n;
        obj->link_flags++;
        n->spawn_x = 0x60;
        n->spawn_y = 0x1e0;
        n->field_90 = obj->field_90;
        n->data_ptr_a = obj->data_ptr_a;
        n->data_ptr_b = obj->data_ptr_b;
    }
}
