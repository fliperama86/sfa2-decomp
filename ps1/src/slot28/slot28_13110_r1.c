/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051be4_slot28[];
extern ObjectRef data_80051c0c_slot28;
extern Object *data_80051c10_slot28[];
extern ObjectRef data_80051c1c_slot28;
extern ObjectRef data_80051c20_slot28;
extern SequenceStep *data_8004a084_slot28[];
extern SequenceStep *data_8004a088_slot28[];
extern SequenceStep *data_8004a094_slot28[];
extern SequenceStep *data_8004a09c_slot28[];
extern SequenceStep *data_8004a0d0_slot28[];
extern u8 data_80048d70_slot28[];
extern u8 data_800490f4_slot28[];
void init_object_data_pointers_a_15_slot28(Object *obj);
void clear_ten_entry_table_13_slot28(void);

void init_slot28_screen_objects_a_slot28(Object *obj) {
    Object *o;
    int one;
    data_8018f5a0->countdown_timer = 0x12c;
    data_8018f5a0->step_index_2 = data_8018f5a0->step_index_2 + 1;
    set_menu_command_one_and_wait_pending(0x405);
    clear_ten_entry_table_13_slot28();
    one = 1;
    start_fade_mode_1_and_wait();
    data_80190568 = one;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        data_80051c0c_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_8004a088_slot28);
        o->field_01 = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        data_80051c20_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_8004a084_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        data_80051c10_slot28[0] = o;
        o->pos_x = 8;
        o->pos_y = -0x60;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 5;
        set_sequence_from_table_5(o, 0, data_8004a094_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        data_80051c1c_slot28.p = o;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x28;
        o->spawn_x = 0;
        o->image_row = 0;
        o->table_index = one;
        o->vel_y = 0xb8;
        o->ot_depth = 0;
        set_sequence_from_table_5(o, 0, data_8004a0d0_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        o->pos_x = 0xdb;
        o->pos_y = 0x5d;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 2;
        set_sequence_from_table_5(o, 7, data_8004a09c_slot28);
        data_80051be4_slot28[0] = o;
        o->ot_depth = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        o->pos_x = 0xd8;
        o->pos_y = 0x69;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 2;
        set_sequence_from_table_5(o, 8, data_8004a09c_slot28);
        data_80051be4_slot28[1] = o;
        o->ot_depth = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        o->pos_x = 0xce;
        o->pos_y = 0x96;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 2;
        set_sequence_from_table_5(o, 9, data_8004a09c_slot28);
        data_80051be4_slot28[2] = o;
        o->ot_depth = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->field_02 = 0x1d;
        o->ot_depth = 2;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x93;
        o->pos_y = 0x69;
        o->field_90 = (void *)0x80060000;
        o->data_ptr_a = data_80048d70_slot28;
        o->data_ptr_b = data_800490f4_slot28;
        o->active_count = one;
        o->table_index = one;
        o->field_01 = one;
        o->image_row = 0;
        o->box_tables = (BoxTables *)data_8004a09c_slot28;
        data_80051be4_slot28[3] = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_15_slot28(o);
        o->pos_x = 0xb0;
        o->pos_y = 0xa1;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 4;
        set_sequence_from_table_5(o, 0, data_8004a09c_slot28);
        data_80051be4_slot28[4] = o;
        o->ot_depth = 3;
    }
    start_fade_mode_c01_and_wait();
}
