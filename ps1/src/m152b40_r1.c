/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void spawn_two_counted_objects_variant_0(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 2; i++) {
        if (data_801a27cc != 0) {
            o = (Object *)pop_block_from_pool();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 10;
                o->table_index = i * 4;
                o->field_0e = object->field_0e;
                o->field_1c = object->field_1c;
            o->countdown = object->kind;
                    o->variant_index = 0;
                o->image_row = 0;
                o->field_90 = (void *)0x800fb100;
                o->spawn_x = 0x60;
                o->spawn_y = 0x1e0;
                o->data_ptr_a = &data_80172a48;
                o->parent = object;
                o->data_ptr_b = &data_80173c9c;
                data_801a27cc--;
            }
        }
    }
}

void spawn_two_counted_objects_variant_2(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 2; i++) {
        if (data_801a27cc != 0) {
            o = (Object *)pop_block_from_pool();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 10;
                o->table_index = i * 4;
                o->field_0e = object->field_0e;
                o->field_1c = object->field_1c;
            o->countdown = object->kind;
                    o->variant_index = 2;
                o->image_row = 0;
                o->field_90 = (void *)0x800fb100;
                o->spawn_x = 0x60;
                o->spawn_y = 0x1e0;
                o->data_ptr_a = &data_80172a48;
                o->parent = object;
                o->data_ptr_b = &data_80173c9c;
                data_801a27cc--;
            }
        }
    }
}

void spawn_three_objects_variant_4(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 3; i++) {
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = 1;
            o->field_02 = 10;
            o->table_index = i * 4;
            o->field_0e = object->field_0e;
            o->field_1c = object->field_1c;
            o->countdown = object->kind;
            o->parent = object;
            o->variant_index = 4;
            o->image_row = 0;
            o->field_90 = (void *)0x800fb100;
            o->spawn_x = 0x60;
            o->spawn_y = 0x1e0;
            o->data_ptr_a = &data_80172a48;
            o->data_ptr_b = &data_80173c9c;
        }
    }
}

void spawn_two_counted_objects_variant_6(Object *object) {
    u16 i;
    Object *o;
    for (i = 0; i < 2; i++) {
        if (data_801a27cc != 0) {
            o = (Object *)pop_block_from_pool();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 10;
                o->table_index = i * 4;
                o->field_0e = object->field_0e;
                o->field_1c = object->field_1c;
            o->countdown = object->kind;
                    o->variant_index = 6;
                o->image_row = 0;
                o->field_90 = (void *)0x800fb100;
                o->spawn_x = 0x60;
                o->spawn_y = 0x1e0;
                o->data_ptr_a = &data_80172a48;
                o->parent = object;
                o->data_ptr_b = &data_80173c9c;
                data_801a27cc--;
            }
        }
    }
}
