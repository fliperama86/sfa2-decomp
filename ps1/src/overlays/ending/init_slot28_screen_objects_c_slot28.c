/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051c4c_slot28;
extern Object *data_80051c50_slot28[];
extern ObjectRef data_80051c5c_slot28;
extern ObjectRef data_80051c60_slot28;
extern SequenceStep *data_8004bd70_slot28[];
extern SequenceStep *data_8004bd74_slot28[];
extern SequenceStep *data_8004bd7c_slot28[];
extern SequenceStep *data_8004bda4_slot28[];
void init_object_data_pointers_a_16_slot28(Object *o);
void clear_ten_entry_table_14_slot28(void);

void init_slot28_screen_objects_c_slot28(Object *obj) {
    HudState *g = data_8018f5a0;
    Object *p;
    int one;
    g->countdown_timer = 0x258;
    g->step_index_2 += 1;
    set_menu_command_one_and_wait_pending(0x404);
    clear_ten_entry_table_14_slot28();
    one = 1;
    start_fade_mode_1_and_wait();
    data_80190568 = one;
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_16_slot28(p);
        data_80051c4c_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 0;
        set_sequence_from_table_5(p, 1, data_8004bd74_slot28);
        p->field_01 = one;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_16_slot28(p);
        data_80051c60_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 0;
        set_sequence_from_table_5(p, 0, data_8004bd70_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_16_slot28(p);
        data_80051c50_slot28[0] = p;
        p->pos_x = 0x58;
        p->pos_y = 0x20;
        p->spawn_x = 0x10;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 5;
        set_sequence_from_table_5(p, 0, data_8004bd7c_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_16_slot28(p);
        data_80051c5c_slot28.p = p;
        p->spawn_y = 0x1e0;
        p->pos_x = 0x28;
        p->spawn_x = 0;
        p->image_row = 0;
        p->table_index = one;
        p->vel_y = 0xb8;
        p->ot_depth = 0;
        set_sequence_from_table_5(p, 0, data_8004bda4_slot28);
    }
    start_fade_mode_c01_and_wait();
}
