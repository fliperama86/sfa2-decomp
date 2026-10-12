/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_8005197c_slot28;
extern Object *data_80051980_slot28[];
extern ObjectRef data_80051984_slot28;
extern ObjectRef data_80051994_slot28;
extern Object *data_80051954_slot28[];
extern ObjectRef data_80051958_slot28;
extern u8 data_80032a64_slot28[];
extern u8 data_80032ec8_slot28[];
extern SequenceStep *data_80034518_slot28[];
extern SequenceStep *data_80034520_slot28[];
extern SequenceStep *data_80034530_slot28[];
void release_active_objects_in_table_3_slot28(void);
void init_object_data_pointers_a_5_slot28(Object *obj);
void set_sequence_of_fourth_hud_object_5_slot28(Object *obj, int arg);

void start_pass_scrolling_objects_spawn_slot28(Object *obj) {
    Object *b;
    Object *prev;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x12c;
        data_8018f5a0->pass_index += 1;
        release_active_objects_in_table_3_slot28();
        set_sequence_from_table_5(data_8005197c_slot28.p, 0, data_80034518_slot28);
        b = data_80051980_slot28[0];
        init_object_data_pointers_a_5_slot28(b);
        b->pos_x = 0x60;
        b->pos_y = 0x20;
        b->spawn_x = 0x10;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 4;
        set_sequence_from_table_5(b, 0, data_80034520_slot28);
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            init_object_data_pointers_a_5_slot28(b);
            data_80051994_slot28.p = b;
            b->pos_x = -0xa0;
            b->pos_y = 0x20;
            b->spawn_x = 0x10;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 4;
            set_sequence_from_table_5(b, 1, data_80034520_slot28);
        }
        b = data_80051984_slot28.p;
        b->field_01 = 0;
        prev = b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x20;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80032a64_slot28;
            b->data_ptr_b = data_80032ec8_slot28;
            b->pos_x = 0xf8;
            b->pos_y = 0xa0;
            b->countdown = 0x100;
            b->table_index = 4;
            b->field_01 = 1;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 2;
            set_sequence_from_table_5(b, 5, data_80034530_slot28);
            data_80051954_slot28[0] = b;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x20;
            b->table_index = 5;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80032a64_slot28;
            b->data_ptr_b = data_80032ec8_slot28;
            b->pos_x = 0x150;
            b->pos_y = 0xa0;
            b->countdown = 0x100;
            b->field_01 = 1;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 3;
            b->variant_index = 0;
            set_sequence_from_table_5(b, 6, data_80034530_slot28);
            data_80051958_slot28.p = b;
            b->parent = prev;
            b->box_tables = (BoxTables *)data_80034530_slot28;
        }
        start_fade_mode_1a01_and_wait();
        set_sequence_of_fourth_hud_object_5_slot28(obj, 3);
    }
}
