/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4e5c_slot04_0b[];
extern u8 data_801b7274_slot04_0b[];
extern u32 data_801c23e0_slot04_0b[];

void noop_slot00_2_slot04_0b(Object *obj);

void init_fighter_and_spawn_children_slot04_0b(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    Object *d;

    dst = (u32 *)0x1f800100;
    obj->data_ptr_a = data_801b4e5c_slot04_0b;
    obj->data_ptr_b = data_801b7274_slot04_0b;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c23e0_slot04_0b[i];
    }
    d = pop_object_from_second_pool();
    if (d != 0) {
        d->active_count = 1;
        d->field_02 = 0xb;
        d->table_index = obj->kind;
        d->parent = obj;
        d->field_0c = obj->field_0c;
        d->field_0e = obj->field_0e;
        obj->linked_object = (u32)d;
        d->sound_side = obj->side;
        d->image_row = obj->image_row;
        d->spawn_x = obj->spawn_x;
        d->spawn_y = obj->spawn_y;
        d->field_90 = obj->field_90;
        d->data_ptr_a = obj->data_ptr_a;
        d->data_ptr_b = obj->data_ptr_b;
    }
    if (game_state.count_index == 0) {
        if (obj->other->kind == 0) {
            if (obj->mode_flag_2 != 0) {
                obj->state_2 = 1;
                if (obj->side == 0) {
                    obj->pos_x = obj->pos_x + 0x50;
                } else {
                    obj->pos_x = obj->pos_x - 0x50;
                }
                noop_slot00_2_slot04_0b(obj);
                c = (Object *)pop_block_from_pool();
                if (c != 0) {
                    c->active_count = 1;
                    c->field_02 = 6;
                    c->table_index = 0;
                    c->parent = obj;
                    c->mode_id = 0x20;
                    c->field_90 = obj->field_90;
                    c->spawn_x = obj->spawn_x;
                    c->spawn_y = obj->spawn_y;
                    c->sound_side = obj->side;
                    c->data_ptr_a = obj->data_ptr_a;
                    c->data_ptr_b = obj->data_ptr_b;
                }
                c = (Object *)pop_block_from_pool();
                if (c != 0) {
                    c->active_count = 1;
                    c->field_02 = 6;
                    c->table_index = 1;
                    c->parent = obj;
                    c->mode_id = 0x20;
                    c->sound_side = obj->side;
                    c->field_90 = obj->field_90;
                    c->data_ptr_a = obj->data_ptr_a;
                    c->data_ptr_b = obj->data_ptr_b;
                    if (c->sound_side == 0) {
                        c->spawn_x = 0x110;
                    } else {
                        c->spawn_x = 0x130;
                    }
                    c->spawn_y = 0x1e0;
                }
                set_sequence_by_side(obj, 0x30);
            } else {
                obj->state_2 = 2;
                set_sequence_by_side(obj, 0x32);
            }
        } else {
            obj->state_2 = 0;
        }
    } else {
        obj->state_2 = 0;
    }
}
