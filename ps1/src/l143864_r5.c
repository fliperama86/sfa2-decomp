/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy) {
    Object *o;
    s16 x;

    x = dx;
    o = (Object *)pop_block_from_pool();
    if (o != 0 && data_801a27d4 != 0) {
        o->active_count = 1;
        o->field_02 = 4;
        o->field_0e = object->field_0e;
        o->variant_index = 0;
        o->team_id = object->team_id;
        o->parent = object->other;
        o->pos_x = object->pos_x;
        o->pos_y = object->pos_y;
        o->facing_side = object->facing_side;
        if (o->facing_side != 0) {
            x = -dx;
        }
        o->table_index = a;
        o->field_01 = 0;
        o->base_x = 0x240;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->field_90 = (void *)0x800fb100;
        o->base_y = 0;
        o->pos_x = o->pos_x + x;
        o->pos_y = o->pos_y - dy;
        data_801a27d4--;
    }
}

void spawn_effect_at_other_offset(Object *object, int a, int dx, int dy) {
    Object *o;
    Object *other;

    o = (Object *)pop_block_from_pool();
    if (o != 0 && data_801a27d4 != 0) {
        other = object->other;
        o->active_count = 1;
        o->field_02 = 4;
        o->field_0e = other->field_0e;
        o->variant_index = 0;
        o->team_id = object->team_id;
        o->parent = other;
        o->pos_x = other->pos_x;
        o->pos_y = other->pos_y;
        o->facing_side = other->facing_side;
        if (o->facing_side != 0) {
            dx = -dx;
        }
        o->table_index = a;
        o->field_01 = 0;
        o->base_x = 0x240;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->data_ptr_a = &data_80172a48;
        o->data_ptr_b = &data_80173c9c;
        o->field_90 = (void *)0x800fb100;
        o->base_y = 0;
        o->pos_x = o->pos_x + dx;
        o->pos_y = o->pos_y - dy;
        data_801a27d4--;
    }
}
