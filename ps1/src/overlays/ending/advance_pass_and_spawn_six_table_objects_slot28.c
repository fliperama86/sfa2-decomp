/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051998_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519a0_slot28;
extern ObjectRef data_800519a4_slot28;
extern ObjectRef data_800519a8_slot28;
extern ObjectRef data_800519ac_slot28;
extern Object *data_800519c4_slot28[];
extern ObjectRef data_800519c8_slot28;
extern SequenceStep *data_8003669c_slot28[];
extern SequenceStep *data_800366a8_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
void release_active_objects_in_table_4_slot28(void);

void advance_pass_and_spawn_six_table_objects_slot28(Object *obj) {
    Object *p;
    Object *q;
    int five;
    if (obj->field_f0 == 0) {
        HudState *h = data_8018f5a0;
        h->countdown_timer = 0xf0;
        h->pass_index = h->pass_index + 1;
        release_active_objects_in_table_4_slot28();
        five = 5;
        p = data_800519c4_slot28[0];
        p->pos_y = 0x20;
        p->ot_depth = five;
        set_sequence_from_table_5(p, 1, data_8003669c_slot28);
        p = data_800519c8_slot28.p;
        p->field_01 = 0;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x33;
            p->table_index = 3;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_80035430_slot28;
            p->data_ptr_b = data_800357d8_slot28;
            p->pos_x = 0x70;
            p->pos_y = 0x70;
            p->countdown = 0x1000;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 4;
            p->field_01 = 0;
            p->image_row = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_80051998_slot28[0] = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x33;
            p->table_index = 4;
            p->field_01 = 0;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_80035430_slot28;
            p->data_ptr_b = data_800357d8_slot28;
            p->pos_x = 0x90;
            p->pos_y = 0x60;
            p->countdown = 0x2000;
            p->spawn_y = 0x1e0;
            p->spawn_x = 0x60;
            p->image_row = 0;
            p->ot_depth = 4;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_8005199c_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x33;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_80035430_slot28;
            p->data_ptr_b = data_800357d8_slot28;
            p->pos_x = 0x90;
            p->pos_y = 0x70;
            p->countdown = 0x3000;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 4;
            p->table_index = five;
            p->field_01 = 0;
            p->image_row = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519a0_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x33;
            p->table_index = 6;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_80035430_slot28;
            p->data_ptr_b = data_800357d8_slot28;
            p->pos_x = 0xa0;
            p->pos_y = 0x58;
            p->countdown = 0x4000;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 4;
            p->field_01 = 0;
            p->image_row = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519a4_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        q = p;
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x33;
            p->table_index = 7;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_80035430_slot28;
            p->data_ptr_b = data_800357d8_slot28;
            p->pos_x = -0xb0;
            p->pos_y = 0x130;
            p->countdown = 0x4000;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 2;
            p->field_01 = 0;
            p->image_row = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519a8_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x33;
            p->table_index = 8;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_80035430_slot28;
            p->data_ptr_b = data_800357d8_slot28;
            p->pos_x = 0xa0;
            p->pos_y = 0x58;
            p->countdown = 0x4000;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 3;
            p->field_01 = 0;
            p->image_row = 0;
            p->box_tables = (BoxTables *)data_800366a8_slot28;
            data_800519ac_slot28.p = p;
            p->linked_object = (u32)q;
        }
        start_fade_mode_1a01_and_wait();
    }
}
