/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_linked_pooled_object_health_0_slot04_09(Object *obj) {
    Object *p;

    p = pop_object_from_pool(obj);
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 9;
        p->table_index = 0;
        p->sound_side = obj->sound_side;
        p->team_id = obj->team_id;
        p->field_49 = obj->field_49;
        p->vel_table_index = obj->direction_index;
        p->table_index_3 = obj->variant_index_2;
        p->active_flag_2 = 0;
        p->field_0e = obj->field_0e;
        p->facing_side = obj->facing_side;
        p->field_0c = obj->field_0c;
        p->image_row = obj->image_row;
        p->scroll_factor = obj->scroll_factor;
        p->health = 0;
        *(u32 *)&p->field_a4 = 0x1000700;
        p->parent = obj;
        obj->linked_object_3 = (s32)p;
        obj->link_flags++;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->field_90 = obj->field_90;
        p->data_ptr_a = obj->data_ptr_a;
        p->data_ptr_b = obj->data_ptr_b;
    }
}
