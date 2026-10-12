/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051c64_slot28[];
extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
extern SequenceStep *data_8004dce4_slot28[];
extern u8 data_8004c73c_slot28[];
extern u8 data_8004cbc4_slot28[];
extern SequenceStep *data_8004dce8_slot28[];
extern SequenceStep *data_8004dd1c_slot28[];
void release_active_objects_in_table_15_slot28(void);
void set_sequence_of_fourth_hud_object_17_slot28(Object *obj, int arg);

void advance_pass_spawn_slot28_objects_f_slot28(Object *obj) {
    HudState *h;
    Object *b;
    Object *prev;
    int i;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 0x258;
        h->pass_index++;
        release_active_objects_in_table_15_slot28();
        b = data_80051c90_slot28[0];
        b->ot_depth = 5;
        b->pos_x = 0x58;
        b->field_01 = 0;
        b->pos_y = -0x60;
        b = data_80051c94_slot28.p;
        b->pos_x = 0x50;
        b->pos_y = 0x20;
        b->spawn_x = 0x20;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 4;
        b->field_01 = 1;
        set_sequence_from_table_5(b, 0, data_8004dce4_slot28);
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->field_02 = 0x2d;
            b->ot_depth = 3;
            b->spawn_x = 0x60;
            b->pos_x = 0xb8;
            b->pos_y = 0xa0;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_8004c73c_slot28;
            b->data_ptr_b = data_8004cbc4_slot28;
            b->active_count = 1;
            b->table_index = 1;
            b->field_01 = 1;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->box_tables = (BoxTables *)data_8004dce8_slot28;
            data_80051c64_slot28[1] = b;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->field_02 = 0x2d;
            b->ot_depth = 2;
            b->spawn_x = 0x60;
            b->pos_x = -8;
            b->pos_y = 0xa0;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_8004c73c_slot28;
            b->data_ptr_b = data_8004cbc4_slot28;
            b->active_count = 1;
            b->table_index = 0;
            b->field_01 = 1;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->box_tables = (BoxTables *)data_8004dce8_slot28;
            prev = data_80051c64_slot28[1];
            data_80051c64_slot28[0] = b;
            ((ObjectWords *)b)->word_58 = (s32)data_8004dd1c_slot28;
            b->parent = prev;
        }
        for (i = 0; i < 6; i++) {
            b = (Object *)pop_block_from_pool();
            if (b != 0) {
                b->field_02 = 0x2d;
                b->table_index = 3;
                b->ot_depth = 4;
                b->spawn_x = 0x60;
                b->spawn_y = 0x1e0;
                b->field_90 = (void *)0x80060000;
                b->data_ptr_a = data_8004c73c_slot28;
                b->data_ptr_b = data_8004cbc4_slot28;
                b->active_count = 1;
                b->field_01 = 1;
                b->image_row = 0;
                b->box_tables = (BoxTables *)data_8004dce8_slot28;
                data_80051c64_slot28[i + 2] = b;
            }
        }
        set_sequence_of_fourth_hud_object_17_slot28(obj, 2);
        start_fade_mode_1a01_and_wait();
    }
}
