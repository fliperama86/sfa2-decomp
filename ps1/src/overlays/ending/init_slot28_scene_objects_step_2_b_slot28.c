/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051a80_slot28;
extern Object *data_80051a84_slot28[];
extern ObjectRef data_80051a90_slot28;
extern ObjectRef data_80051a94_slot28;
extern Object *data_80051a58_slot28[];
extern SequenceStep *data_8003d2cc_slot28[];
extern SequenceStep *data_8003d2d0_slot28[];
extern SequenceStep *data_8003d2d8_slot28[];
extern SequenceStep *data_8003d2e0_slot28[];
extern SequenceStep *data_8003d2f4_slot28[];
void init_object_data_pointers_a_9_slot28(Object *o);
void clear_ten_entry_table_7_slot28(void);

void init_slot28_scene_objects_step_2_b_slot28(Object *obj) {
    HudState *g = screen_state;
    Object *p;
    int one;
    g->countdown_timer = 0x258;
    g->step_index_2 += 1;
    clear_ten_entry_table_7_slot28();
    start_fade_mode_1_and_wait();
    one = 1;
    kind_handlers_enabled = one;
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        data_80051a80_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        set_sequence_from_table_5(p, 0, data_8003d2d0_slot28);
        p->field_01 = one;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        data_80051a94_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        set_sequence_from_table_5(p, 0, data_8003d2cc_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        data_80051a84_slot28[0] = p;
        p->pos_x = 0x58;
        p->pos_y = 0x20;
        p->spawn_x = 0x10;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 5;
        set_sequence_from_table_5(p, 0, data_8003d2d8_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        data_80051a90_slot28.p = p;
        p->spawn_y = 0x1e0;
        p->pos_x = 0x28;
        p->spawn_x = 0;
        p->image_row = 0;
        p->table_index = one;
        p->vel_y = 0xb8;
        p->ot_depth = 0;
        set_sequence_from_table_5(p, 0, data_8003d2f4_slot28);
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        p->pos_x = 0xb8;
        p->pos_y = 0x50;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 2;
        set_sequence_from_table_5(p, 0, data_8003d2e0_slot28);
        data_80051a58_slot28[0] = p;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        p->pos_x = 0xc8;
        p->pos_y = 0x58;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 2;
        set_sequence_from_table_5(p, 1, data_8003d2e0_slot28);
        data_80051a58_slot28[1] = p;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        p->pos_x = 0xd8;
        p->pos_y = 0x68;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 2;
        set_sequence_from_table_5(p, 2, data_8003d2e0_slot28);
        data_80051a58_slot28[2] = p;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        init_object_data_pointers_a_9_slot28(p);
        p->pos_x = 0xee;
        p->pos_y = 0x78;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = 2;
        set_sequence_from_table_5(p, 3, data_8003d2e0_slot28);
        data_80051a58_slot28[3] = p;
    }
    set_menu_command_one_and_wait_pending(0x302);
    start_fade_mode_c01_and_wait();
}
