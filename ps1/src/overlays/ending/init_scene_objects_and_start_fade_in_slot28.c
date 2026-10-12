/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_80036690_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
extern Object *data_80051998_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519c0_slot28;
extern Object *data_800519c4_slot28[];
extern ObjectRef data_800519c8_slot28;
extern ObjectRef data_800519d0_slot28;
extern ObjectRef data_800519d4_slot28;
void init_object_data_pointers_a_6_slot28(Object *obj);
void clear_ten_entry_table_4_slot28(void);

void init_scene_objects_and_start_fade_in_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int one;
    Object *o;
    h->countdown_timer = 0xb4;
    h->step_index_2++;
    set_menu_command_one_and_wait_pending(0x501);
    one = 1;
    clear_ten_entry_table_4_slot28();
    start_fade_mode_1_and_wait();
    data_80190568 = one;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_6_slot28(o);
        data_800519c0_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 1, data_80036690_slot28 + 1);
        o->field_01 = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_6_slot28(o);
        data_800519d4_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_80036690_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_6_slot28(o);
        data_800519c4_slot28[0] = o;
        o->pos_x = 0x60;
        o->pos_y = -0x60;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 5;
        set_sequence_from_table_5(o, 0, data_80036690_slot28 + 3);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_6_slot28(o);
        data_800519c8_slot28.p = o;
        o->pos_x = 0x60;
        o->pos_y = 0x20;
        o->spawn_x = 0x20;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 4;
        set_sequence_from_table_5(o, 0, data_80036690_slot28 + 5);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_6_slot28(o);
        data_800519d0_slot28.p = o;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x28;
        o->spawn_x = 0;
        o->image_row = 0;
        o->table_index = one;
        o->ot_depth = 0;
        o->vel_y = 0xb8;
        set_sequence_from_table_5(o, 0, data_80036690_slot28 + 20);
        o->field_01 = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        SequenceStep **t = data_80036690_slot28 + 6;
        init_object_data_pointers_a_6_slot28(o);
        o->pos_x = 0xa8;
        o->pos_y = 0x90;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 2;
        set_sequence_from_table_5(o, 0, t);
        data_80051998_slot28[0] = o;
        ((Slot28Obj *)o)->sequence_ptr = t;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->field_02 = 0x33;
        o->field_90 = (void *)0x80060000;
        o->data_ptr_a = data_80035430_slot28;
        o->data_ptr_b = data_800357d8_slot28;
        o->pos_x = 0xe8;
        o->pos_y = 0x91;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->ot_depth = 3;
        o->active_count = one;
        o->table_index = 0;
        o->field_01 = 0;
        o->image_row = 0;
        data_8005199c_slot28.p = o;
        ((Slot28Obj *)o)->sequence_ptr = data_80036690_slot28 + 6;
    }
    start_fade_mode_c01_and_wait();
}
