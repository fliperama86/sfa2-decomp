/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern SequenceStep *data_800240f8_slot01[];

void func_80013f78_slot01(Object *obj) {
    Rect rect;
    Object *a;
    Object *b;
    Object *c;
    int i;
    u8 *base;
    u16 t12;
    int v9;
    obj->state_0 = 2;
    base = (u8 *)&game_state.sub_index;
    i = 0;
    if (*(s16 *)base >= 0) {
    do {
        a = (Object *)func_8011f1e0();
        if (a != 0) {
            a->field_02 = 0x30;
            a->active_count = 1;
            a->field_01 = 1;
            a->table_index = 0x80;
            v9 = *(u8 *)&game_state.sub_index - i + 1;
            a->image_row = 0;
            a->state_0 = 1;
            a->countdown = 0x1f;
            a->ot_depth = v9;
            t12 = obj->pos_x;
            a->pos_y = 0xbc;
            a->health = i << 5;
            a->hit_type = 0x14;
            a->spawn_x = 0x70;
            a->spawn_y = 0x1f4;
            a->data_ptr_a = data_80019330_slot01;
            a->field_90 = (void *)0x80059000;
            a->data_ptr_b = data_800195b8_slot01;
            a->pos_x = t12;
        }
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x30;
            b->field_01 = 1;
            b->table_index = 0x80;
            b->ot_depth = a->ot_depth;
            b->state_0 = 1;
            b->image_row = 0;
            b->countdown = a->countdown;
            b->pos_x = a->pos_x + 2;
            b->pos_y = a->pos_y - 2;
            b->health = a->health;
            b->hit_type = (&game_state.byte_table)[i + 1];
            b->spawn_x = 0x70;
            b->data_ptr_a = data_80019330_slot01;
            b->data_ptr_b = data_800195b8_slot01;
            b->field_90 = (void *)0x80059000;
            if (i != 7) {
                if (i == *(s16 *)&game_state.sub_index) {
                    b->spawn_y = (&game_state.byte_table)[i + 1] + 0x1e0;
                } else {
                    rect.x = b->spawn_x;
                    rect.y = (&game_state.byte_table)[i + 1] + 0x1e0;
                    rect.w = 0x10;
                    rect.h = 1;
                    func_80157fc4(&rect, (u8 *)(0x80071408 + (((&game_state.byte_table)[i + 1] << 5))));
                    func_80157d9c(0);
                    b->spawn_y = (&game_state.byte_table)[i + 1] + 0x1e0;
                    a->spawn_y = 0x1f3;
                    a->hit_type = 0x15;
                }
            } else {
                b->spawn_y = (&game_state.byte_table)[8] + 0x1e0;
            }
            func_80130768(a, a->hit_type, data_800240f8_slot01);
            func_80130768(b, (&game_state.byte_table)[i + 1], data_800240f8_slot01);
        }
        i++;
    } while (i <= *(s16 *)&game_state.sub_index);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->active_count = 1;
        b->field_02 = 0x30;
        b->field_01 = 1;
        b->table_index = 0x80;
        b->ot_depth = 0;
        b->image_row = 0;
        b->state_0 = 1;
        c = b;
        c->countdown = a->countdown;
        c->pos_x = a->pos_x + 2;
        c->pos_y = a->pos_y - 2;
        c->health = a->health;
        c->spawn_x = 0x70;
        c->spawn_y = 0x1f4;
        c->data_ptr_a = data_80019330_slot01;
        c->field_90 = (void *)0x80059000;
        c->data_ptr_b = data_800195b8_slot01;
        func_80130768(c, 0x16, data_800240f8_slot01);
    }
    b = (Object *)func_8011f1e0();
    if (b != 0) {
        b->active_count = 1;
        b->field_02 = 0x30;
        b->field_01 = 1;
        b->table_index = 0x80;
        b->ot_depth = 0;
        b->image_row = 0;
        b->state_0 = 1;
        b->countdown = a->countdown;
        b->pos_x = a->pos_x + 2;
        c = b;
        c->pos_y = a->pos_y - 2;
        c->health = a->health;
        c->spawn_x = 0x70;
        c->spawn_y = 0x1f5;
        c->data_ptr_a = data_80019330_slot01;
        c->field_90 = (void *)0x80059000;
        c->data_ptr_b = data_800195b8_slot01;
        func_80130768(c, (&game_state.byte_table)[i] + 0x17, data_800240f8_slot01);
    }
    if (i < 8) {
        do {
            a = (Object *)func_8011f1e0();
            if (a != 0) {
                a->field_02 = 0x30;
                a->active_count = 1;
                a->field_01 = 1;
                a->table_index = 0x80;
                v9 = i - *(u8 *)&game_state.sub_index + 2;
                a->image_row = 0;
                a->state_0 = 1;
                a->countdown = 0x1f;
                a->ot_depth = v9;
                t12 = obj->pos_x;
                a->pos_y = 0xbc;
                a->health = i << 5;
                a->hit_type = 0x14;
                a->spawn_x = 0x70;
                a->spawn_y = 0x1f4;
                a->data_ptr_a = data_80019330_slot01;
                a->field_90 = (void *)0x80059000;
                a->data_ptr_b = data_800195b8_slot01;
                a->pos_x = t12;
            }
            b = (Object *)func_8011f1e0();
            if (b != 0) {
                b->active_count = 1;
                b->field_02 = 0x30;
                b->field_01 = 1;
                b->table_index = 0x80;
                b->ot_depth = a->ot_depth;
                b->image_row = 0;
                b->state_0 = 1;
                b->countdown = a->countdown;
                b->pos_x = a->pos_x + 2;
                b->pos_y = a->pos_y - 2;
                b->health = a->health;
                b->step_flags = (&game_state.byte_table)[i + 1];
                b->spawn_x = 0x70;
                if (((game_state.bit_mask >> (&game_state.byte_table)[i + 1]) & 1) && i < 7) {
                    func_80130768(b, (&game_state.byte_table)[i + 1], data_800240f8_slot01);
                    rect.x = b->spawn_x;
                    rect.y = (&game_state.byte_table)[i + 1] + 0x1e0;
                    rect.w = 0x10;
                    rect.h = 1;
                    func_80157fc4(&rect, (u8 *)(0x80071408 + (((&game_state.byte_table)[i + 1] << 5))));
                    func_80157d9c(0);
                    b->spawn_y = (&game_state.byte_table)[i + 1] + 0x1e0;
                    a->spawn_y = 0x1f3;
                    a->hit_type = 0x15;
                } else {
                    func_80130768(b, 0x13, data_800240f8_slot01);
                    b->spawn_y = 0x1f3;
                }
                func_80130768(a, a->hit_type, data_800240f8_slot01);
                b->field_90 = (void *)0x80059000;
                b->data_ptr_a = data_80019330_slot01;
                b->data_ptr_b = data_800195b8_slot01;
            }
            i++;
        } while (i < 8);
    }
}
