/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_80031c90_slot28[];
extern SequenceStep *data_80031c8c_slot28[];
extern SequenceStep *data_80031c9c_slot28[];
extern SequenceStep *data_80031ca0_slot28[];
extern SequenceStep *data_80031cc0_slot28[];
extern Object *data_80051910_slot28[];
extern int data_80051950_slot28;
void init_object_data_pointers_a_4_slot28(Object *obj);
void clear_ten_entry_table_2_slot28(void);

void setup_pooled_objects_for_menu_screen_b_slot28(Object *obj) {
    Object *o;
    int one;
    HudState *h = screen_state;
    h->countdown_timer = 0x258;
    h->step_index_2 += 1;
    clear_ten_entry_table_2_slot28();
    start_fade_mode_1_and_wait();
    one = 1;
    kind_handlers_enabled = one;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_4_slot28(o);
        data_80051910_slot28[10] = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_80031c90_slot28);
        o->field_01 = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_4_slot28(o);
        data_80051910_slot28[15] = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_80031c8c_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_4_slot28(o);
        data_80051910_slot28[12] = o;
        o->pos_x = 0x60;
        o->pos_y = -0x60;
        o->spawn_x = 0x20;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 4;
        set_sequence_from_table_5(o, 0, data_80031c9c_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_4_slot28(o);
        o->pos_x = 0xc0;
        o->pos_y = 0x90;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_80031ca0_slot28);
        data_80051910_slot28[0] = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_4_slot28(o);
        data_80051910_slot28[14] = o;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x28;
        o->spawn_x = 0;
        o->image_row = 0;
        o->table_index = one;
        o->ot_depth = 0;
        o->vel_y = 0xb8;
        set_sequence_from_table_5(o, 0, data_80031cc0_slot28);
    }
    set_menu_command_one_and_wait_pending(0x301);
    start_fade_mode_c01_and_wait();
    data_80051950_slot28 = 0;
}
