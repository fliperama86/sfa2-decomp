/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
extern ObjectRef data_80051b48_slot28;
extern ObjectRef data_80051b50_slot28;
extern ObjectRef data_80051b54_slot28;
extern u8 data_80051b58_slot28;
extern u8 data_80051b5c_slot28;
extern u8 data_80051b60_slot28;
extern SequenceStep *data_8004384c_slot28[];
extern SequenceStep *data_80043848_slot28[];
extern SequenceStep *data_80043858_slot28[];
extern SequenceStep *data_80043860_slot28[];
extern SequenceStep *data_80043870_slot28[];
void clear_ten_entry_table_10_slot28(void);
void init_object_data_pointers_a_12_slot28(Object *obj);

void init_slot28_scene_b_objects_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *b;
    int one = 1;
    h->countdown_timer = 0xb4;
    h->step_index_2++;
    set_menu_command_one_and_wait_pending(0x306);
    clear_ten_entry_table_10_slot28();
    start_fade_mode_1_and_wait();
    data_80190568 = one;
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_12_slot28(b);
        data_80051b40_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        set_sequence_from_table_5(b, 1, data_8004384c_slot28);
        b->field_01 = one;
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_12_slot28(b);
        data_80051b54_slot28.p = b;
        b->pos_x = -8;
        b->pos_y = -8;
        b->spawn_x = 0;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = one;
        set_sequence_from_table_5(b, 0, data_80043848_slot28);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_12_slot28(b);
        data_80051b44_slot28[0] = b;
        b->pos_x = 0x58;
        b->pos_y = 0x20;
        b->spawn_x = 0x10;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 5;
        set_sequence_from_table_5(b, 0, data_80043858_slot28);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_12_slot28(b);
        data_80051b48_slot28.p = b;
        b->pos_x = 0x58;
        b->pos_y = -0x60;
        b->spawn_x = 0x20;
        b->spawn_y = 0x1e0;
        b->image_row = 0;
        b->ot_depth = 4;
        set_sequence_from_table_5(b, 0, data_80043860_slot28);
    }
    b = (Object *)pop_block_from_pool();
    if (b != 0) {
        init_object_data_pointers_a_12_slot28(b);
        data_80051b50_slot28.p = b;
        b->spawn_y = 0x1e0;
        b->pos_x = 0x28;
        b->spawn_x = 0;
        b->image_row = 0;
        b->table_index = one;
        b->vel_y = 0xb8;
        b->ot_depth = 0;
        set_sequence_from_table_5(b, 0, data_80043870_slot28);
    }
    data_80051b58_slot28 = 5;
    data_80051b5c_slot28 = 0x20;
    data_80051b60_slot28 = 0;
    start_fade_mode_c01_and_wait();
}
