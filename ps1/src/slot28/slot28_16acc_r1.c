/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051cd0_slot28[];
extern Object *data_80051ca4_slot28[];
extern SequenceStep *data_800502d4_slot28[];
extern SequenceStep *data_800502e8_slot28[];
extern u8 data_8004ead4_slot28[];
extern u8 data_8004ef00_slot28[];
void release_active_objects_in_table_16_slot28(void);
void set_sequence_of_fourth_hud_object_18_slot28(Object *o, int arg);
void init_object_data_pointers_a_18_slot28(Object *o);

void advance_pass_spawn_slot28_objects_i_slot28(Object *obj) {
    HudState *h;
    Object *p;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->pass_index++;
        release_active_objects_in_table_16_slot28();
        set_sequence_of_fourth_hud_object_18_slot28(obj, 2);
        p = data_80051cd0_slot28[0];
        p->pos_x = 0x38;
        p->pos_y = 0x10;
        p->ot_depth = 8;
        p->field_01 = 1;
        set_sequence_from_table_5(p, 2, data_800502d4_slot28);
        p->vel_y = 0x40000;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_18_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0xa0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 4;
            set_sequence_from_table_5(p, 3, data_800502e8_slot28);
            data_80051ca4_slot28[4] = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->vel_y = 0x40000;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_18_slot28(p);
            p->pos_x = 0x108;
            p->pos_y = 0xa0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 3;
            set_sequence_from_table_5(p, 5, data_800502e8_slot28);
            data_80051ca4_slot28[5] = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->vel_y = 0x40000;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->table_index = 0;
            p->field_01 = 1;
            p->field_02 = 0x73;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8004ead4_slot28;
            p->data_ptr_b = data_8004ef00_slot28;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->image_row = 0;
            p->ot_depth = 2;
            data_80051ca4_slot28[0] = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->table_index = 1;
            p->field_01 = 1;
            p->field_02 = 0x73;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8004ead4_slot28;
            p->data_ptr_b = data_8004ef00_slot28;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->image_row = 0;
            p->ot_depth = 2;
            data_80051ca4_slot28[1] = p;
        }
        start_fade_mode_1a01_and_wait();
    }
}
