/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80146ba0(Object *object, u8 index) {
    s8 tbl[7] = {0x15, 0x16, 0x30, 0x0c, 0x0d, 0x0a, 0x12};
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
    ref_other.p->active_count = 1;
    ref_other.p->field_02 = 5;
    ref_other.p->table_index = 0;
    ref_other.p->field_0e = object->field_0e;
    ref_other.p->ot_depth = 4;
    ref_other.p->variant_index = tbl[index];
    ref_other.p->pos_x = object->pos_x;
    ref_other.p->pos_y = object->pos_y;
    ref_other.p->facing_side = object->facing_side;
    ref_other.p->field_1c = object->field_1c;
    ref_other.p->field_0c = 0;
    return 1;
    }
    return 0;
}

int func_80146cf8(Object *object, u8 index) {
    s8 tbl[2] = {0x10, 0x11};
    Object *other;
    other = object->other;
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
    ref_other.p->active_count = 1;
    ref_other.p->field_02 = 5;
    ref_other.p->table_index = tbl[index];
    ref_other.p->field_0e = other->field_0e;
    ref_other.p->ot_depth = 0;
    ref_other.p->variant_index = 0x16;
    ref_other.p->pos_x = other->pos_x;
    ref_other.p->pos_y = other->ground_y;
    ref_other.p->facing_side = 0;
    ref_other.p->field_1c = object->field_1c;
    ref_other.p->field_0c = 0;
    return 1;
    }
    return 0;
}

int func_80146e3c(Object *object, u8 index) {
    s8 tbl[10] = {0x10, 0x11, 0x0f, 0x0b, 0x0c, 0x0d, 0x0e, 0x20, 0x21, 0x22};
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
    ref_other.p->active_count = 1;
    ref_other.p->field_02 = 5;
    ref_other.p->table_index = 0;
    ref_other.p->field_0e = object->field_0e;
    ref_other.p->ot_depth = 0;
    ref_other.p->variant_index = tbl[index];
    ref_other.p->facing_side = object->facing_side;
    ref_other.p->pos_x = object->pos_x;
    ref_other.p->pos_y = object->pos_y;
    ref_other.p->field_1c = object->field_1c;
    ref_other.p->field_0c = 0;
    ref_other.p->pos_x = ref_other.p->pos_x + table_8017cac4[get_random_byte() & 0xf];
    ref_other.p->pos_y = ref_other.p->pos_y - table_8017cac4[get_random_byte() & 0xf];
    return 1;
    }
    return 0;
}

int func_80147000(Object *object) {
    Object *o;
    Object *src;
    u8 w;
    u16 v;

    if (game_state.spawn_flag == 0) {
        game_state.spawn_flag = 0xff;
        o = (Object *)func_8011f1e0();
        if (o == 0) {
            return 0;
        }
        src = object->mode_id == 8 ? object->parent : object;
        o->active_count = 1;
        if (game_state.spawn_kind == 6) {
            func_8014780c(object);
        }
        o->field_02 = 6;
        o->table_index = game_state.spawn_kind;
        o->parent = object;
        o->active_flag = src->facing_side;
        o->field_1c = src->field_1c;
        w = src->field_255;
        o->data_ptr_a = &data_80172a48;
        o->data_ptr_b = &data_80173c9c;
        o->field_90 = (void *)0x800fb100;
        o->pos_x = 0;
        o->pos_y = 0;
        o->ot_depth = 7;
        o->active_state = w;
        v = game_state.field_dc;
        game_state.field_64 = 0x90;
        o->spawn_x = 0x20;
        o->spawn_y = 0x1e0;
        data_80189454 = v;
        o->image_row = 0;
        if (game_state.spawn_kind == 6) {
            o->state_0 = 4;
        }
        return 1;
    }
    return 0;
}
