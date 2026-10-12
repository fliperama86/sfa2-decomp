/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051c8c_slot28;
extern Object *data_80051c90_slot28[];
extern ObjectRef data_80051c94_slot28;
extern ObjectRef data_80051c9c_slot28;
extern ObjectRef data_80051ca0_slot28;
extern Object *data_80051c64_slot28[];
extern SequenceStep *data_8004dcd4_slot28[];
extern SequenceStep *data_8004dcd8_slot28[];
extern SequenceStep *data_8004dce0_slot28[];
extern SequenceStep *data_8004dce8_slot28[];
extern SequenceStep *data_8004dd0c_slot28[];
extern SequenceStep *data_8004dd1c_slot28[];
void init_object_data_pointers_a_17_slot28(Object *obj);
void clear_ten_entry_table_15_slot28(void);

void init_slot28_screen_objects_e_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->countdown_timer = 0x258;
    h->step_index_2++;
    set_menu_command_one_and_wait_pending(0x207);
    clear_ten_entry_table_15_slot28();
    start_fade_mode_1_and_wait();
    data_80190568 = one;
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        data_80051c8c_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 0;
        set_sequence_from_table_5(b, 0, data_8004dcd8_slot28);
        b->field_01 = one;
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        data_80051ca0_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 0;
        set_sequence_from_table_5(b, 0, data_8004dcd4_slot28);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        data_80051c90_slot28[0] = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->spawn_x = 0x10;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 4;
        set_sequence_from_table_5(b, 0, data_8004dce0_slot28);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        data_80051c94_slot28.p = b;
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        data_80051c9c_slot28.p = b;
        b->spawn_y = 0x1e0;
        b->pos_x = 0x28;
        b->spawn_x = 0;
        b->image_row = 0;
        b->table_index = one;
        b->vel_y = 0xb8;
        b->ot_depth = 0;
        set_sequence_from_table_5(b, 0, data_8004dd0c_slot28);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        b->pos_x = 0xe0;
        b->pos_y = 0x60;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 2;
        set_sequence_from_table_5(b, 0, data_8004dd1c_slot28);
        data_80051c64_slot28[0] = b;
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        b->pos_x = 0xdb;
        b->pos_y = 0x65;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 2;
        set_sequence_from_table_5(b, 0, data_8004dd1c_slot28);
        data_80051c64_slot28[1] = b;
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        b->pos_x = 0xd0;
        b->pos_y = 0x80;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 2;
        set_sequence_from_table_5(b, 2, data_8004dd1c_slot28);
        data_80051c64_slot28[2] = b;
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_17_slot28(b);
        b->pos_x = 0xb8;
        b->pos_y = 0x90;
        b->spawn_x = 0x60;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 3;
        set_sequence_from_table_5(b, 0, data_8004dce8_slot28);
        data_80051c64_slot28[3] = b;
    }
    start_fade_mode_c01_and_wait();
}
