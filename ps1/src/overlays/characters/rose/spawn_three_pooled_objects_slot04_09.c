/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_three_pooled_objects_slot04_09(Object *obj) {
    Object *a;
    Object *b;
    Object *c;
    Object *t;
    u8 side;
    int three;
    if (data_801a6960 >= 3) {
        side = obj->side;
        three = 3;
        obj->counter = three;
        c = pop_object_from_pool(obj);
        if (c != 0) {
            a = c;
        }
        c = pop_object_from_pool(obj);
        if (c != 0) {
            b = c;
        }
        c = pop_object_from_pool(obj);
        if (c != 0) {
            if (b < a) {
                t = a;
                a = b;
                b = t;
            }
            if (c < a) {
                t = a;
                a = c;
                c = t;
            }
            if (c < b) {
                t = b;
                b = c;
                c = t;
            }
            c->active_count = 1;
            c->field_02 = 0x17;
            c->table_index = 1;
            c->active_flag_2 = 1;
            c->vel_table_index = obj->direction_index;
            c->field_0e = obj->field_0e;
            c->variant_index = side;
            c->sound_side = obj->sound_side;
            c->team_id = obj->team_id;
            c->field_49 = obj->field_49;
            c->field_4b = obj->field_4b;
            c->vel_x = side;
            c->countdown = 8;
            c->field_20 = 8;
            c->parent = obj;
            c->field_0c = obj->field_0c;
            c->image_row = obj->image_row + 3;
            *(s32 *)&c->field_a4 = 0x10;
            c->spawn_x = obj->spawn_x;
            c->spawn_y = obj->spawn_y;
            c->field_90 = obj->field_90;
            c->data_ptr_a = obj->data_ptr_a;
            c->data_ptr_b = obj->data_ptr_b;
            b->active_count = 1;
            b->field_02 = 0x17;
            b->table_index = 2;
            b->active_flag_2 = 1;
            b->vel_table_index = obj->direction_index;
            b->field_0e = obj->field_0e;
            b->variant_index = side;
            b->sound_side = obj->sound_side;
            b->team_id = obj->team_id;
            b->field_49 = obj->field_49;
            b->field_4b = obj->field_4b;
            b->vel_x = side;
            b->countdown = 0x10;
            b->field_20 = 0x10;
            b->parent = obj;
            b->field_0c = obj->field_0c;
            b->image_row = obj->image_row + 4;
            *(s32 *)&b->field_a4 = 8;
            b->spawn_x = obj->spawn_x;
            b->spawn_y = obj->spawn_y;
            b->field_90 = obj->field_90;
            b->data_ptr_a = obj->data_ptr_a;
            b->data_ptr_b = obj->data_ptr_b;
            a->active_count = 1;
            a->field_02 = 0x17;
            a->table_index = three;
            a->active_flag_2 = 1;
            a->vel_table_index = obj->direction_index;
            a->field_0e = obj->field_0e;
            a->variant_index = side;
            a->sound_side = obj->sound_side;
            a->team_id = obj->team_id;
            a->field_49 = obj->field_49;
            a->field_4b = obj->field_4b;
            a->vel_x = side;
            a->countdown = 0x18;
            a->field_20 = 0x18;
            a->parent = obj;
            a->field_0c = obj->field_0c;
            a->image_row = obj->image_row + 4;
            *(s32 *)&a->field_a4 = 0;
            a->spawn_x = obj->spawn_x;
            a->spawn_y = obj->spawn_y;
            a->field_90 = obj->field_90;
            a->data_ptr_a = obj->data_ptr_a;
            a->data_ptr_b = obj->data_ptr_b;
        }
    }
}
