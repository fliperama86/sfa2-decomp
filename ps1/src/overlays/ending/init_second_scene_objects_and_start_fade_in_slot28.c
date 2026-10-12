/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051a00_slot28;
extern Object *data_80051a04_slot28[];
extern ObjectRef data_80051a08_slot28;
extern ObjectRef data_80051a10_slot28;
extern ObjectRef data_80051a14_slot28;
extern Object *data_800519d8_slot28[];
extern SequenceStep *data_80039280_slot28[];
extern SequenceStep *data_8003927c_slot28[];
extern SequenceStep *data_80039298_slot28[];
extern SequenceStep *data_800392d0_slot28[];
extern SequenceStep *data_8003929c_slot28[];

void init_object_data_pointers_a_7_slot28(Object *obj);
void clear_ten_entry_table_5_slot28(void);

void init_second_scene_objects_and_start_fade_in_slot28(Object *obj) {
    Object *p;
    int one;
    int two;
    data_8018f5a0->step_index_2 = data_8018f5a0->step_index_2 + 1;
    clear_ten_entry_table_5_slot28();
    start_fade_mode_1_and_wait();
    one = 1;
    data_80190568 = one;
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_7_slot28(p);
        data_80051a00_slot28.p = p;
        p->pos_y = -8;
        p->pos_x = 0;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        set_sequence_from_table_5(p, 2, data_80039280_slot28);
        p->field_01 = one;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_7_slot28(p);
        data_80051a14_slot28.p = p;
        p->pos_y = -8;
        p->pos_x = 0;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        set_sequence_from_table_5(p, 0, data_8003927c_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_7_slot28(p);
        data_80051a04_slot28[0] = p;
        p->pos_x = 0x50;
        p->pos_y = 0x20;
        p->spawn_x = 0x10;
        p->field_01 = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_7_slot28(p);
        data_80051a08_slot28.p = p;
        p->pos_x = 0x50;
        p->pos_y = 0x20;
        p->spawn_x = 0x20;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 5;
        set_sequence_from_table_5(p, 0, data_80039298_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_7_slot28(p);
        data_80051a10_slot28.p = p;
        p->spawn_y = 0x1e0;
        p->pos_x = 0x28;
        p->spawn_x = 0;
        p->image_row = 0;
        p->table_index = one;
        p->vel_y = 0xb8;
        p->ot_depth = 0;
        set_sequence_from_table_5(p, 0, data_800392d0_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        p->field_02 = 0xa6;
        p->active_count = one;
        p->table_index = 8;
        data_800519d8_slot28[2] = p;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_7_slot28(p);
        p->pos_x = 0xe0;
        p->pos_y = 0x80;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        two = 2;
        p->image_row = 0;
        p->ot_depth = two;
        set_sequence_from_table_5(p, 0, data_8003929c_slot28);
        data_800519d8_slot28[0] = p;
        p->ot_depth = two;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_7_slot28(p);
        p->pos_x = 0xc0;
        p->pos_y = 0x90;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 2;
        set_sequence_from_table_5(p, 0xc, data_8003929c_slot28);
        data_800519d8_slot28[1] = p;
        p->ot_depth = 3;
    }
    set_menu_command_one_and_wait_pending(0x403);
    start_fade_mode_c01_and_wait();
}
