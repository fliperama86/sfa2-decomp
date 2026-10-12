/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void spawn_three_variant_objects_slot12(Object *obj) {
    Object *a;
    obj->state_1 = obj->state_1 + 1;
    a = (Object *)pop_block_from_pool();
    if (a != 0) {
        a->active_count = 1;
        a->field_02 = 0xa2;
        a->variant_index = 0;
        a->ot_depth = 1;
        a->pos_x = obj->spawn_x;
        a->pos_y = obj->spawn_y;
        a->base_y = obj->base_y;
        a->table_index = obj->table_index;
        a->health = obj->health;
    }
    a = (Object *)pop_block_from_pool();
    if (a != 0) {
        a->active_count = 1;
        a->field_02 = 0xa2;
        a->variant_index = 1;
        a->ot_depth = 2;
        a->pos_x = obj->spawn_x;
        a->pos_y = obj->spawn_y;
        a->base_y = obj->base_y;
        a->table_index = obj->table_index;
        a->health = obj->health;
    }
    if (((FrameRecord *)ptr_8019040c)[(s16)obj->health].offset_index == 1) {
        a = (Object *)pop_block_from_pool();
        if (a != 0) {
            a->active_count = 1;
            a->field_02 = 0xa2;
            a->variant_index = 2;
            a->ot_depth = 0;
            a->pos_x = obj->spawn_x;
            a->pos_y = obj->spawn_y;
            a->base_y = obj->base_y;
            a->table_index = obj->table_index;
            a->health = obj->health;
        }
    }
}
