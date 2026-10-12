/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_800518b8_slot28;
extern ObjectRef data_800518cc_slot28;
extern Object *data_800518bc_slot28[];
extern ObjectRef data_800518c8_slot28;
extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern ObjectRef data_80051898_slot28;
extern ObjectRef data_8005189c_slot28;
extern ObjectRef data_800518a0_slot28;
extern SequenceStep *data_8002bdd8_slot28[];
extern SequenceStep *data_8002bdd4_slot28[];
extern SequenceStep *data_8002bddc_slot28[];
extern SequenceStep *data_8002bde4_slot28[];
extern SequenceStep *data_8002be28_slot28[];
extern u8 data_8002acc4_slot28[];
extern u8 data_8002b024_slot28[];

void init_object_data_pointers_a_2_slot28(Object *obj);

void func_800149a0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *o;
    int one;
    h->countdown_timer = 0xf0;
    h->step_index_2 = h->step_index_2 + 1;
    start_fade_mode_1_and_wait();
    one = 1;
    data_80190568 = one;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_2_slot28(o);
        data_800518b8_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 0;
        set_sequence_from_table_5(o, 0, data_8002bdd8_slot28);
        o->field_01 = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_2_slot28(o);
        data_800518cc_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 0;
        set_sequence_from_table_5(o, 0, data_8002bdd4_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_2_slot28(o);
        data_800518bc_slot28[0] = o;
        o->pos_x = 0x60;
        o->pos_y = 0x21;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 2;
        set_sequence_from_table_5(o, 0, data_8002bddc_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_2_slot28(o);
        data_800518c8_slot28.p = o;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x28;
        o->spawn_x = 0;
        o->image_row = 0;
        o->table_index = one;
        o->ot_depth = 0;
        o->vel_y = 0xb8;
        set_sequence_from_table_5(o, 0, data_8002be28_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->field_02 = 0x76;
        o->pos_x = 0xe8;
        o->pos_y = 0xa0;
        o->field_90 = (void *)0x80060000;
        o->data_ptr_a = data_8002acc4_slot28;
        o->data_ptr_b = data_8002b024_slot28;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->countdown = 0x200;
        o->active_count = one;
        o->table_index = 0;
        o->image_row = 0;
        o->field_0c = 0;
        o->ot_depth = one;
        ((Slot28Obj *)o)->sequence_ptr = data_8002bde4_slot28;
        data_80051890_slot28.p = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_2_slot28(o);
        o->ot_depth = 4;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->pos_x = 0xc0;
        o->image_row = 0;
        o->pos_y = 0xa0;
        set_sequence_from_table_5(o, 0, data_8002bde4_slot28);
        data_80051894_slot28.p = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_2_slot28(o);
        o->ot_depth = 3;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x70;
        o->image_row = 0;
        o->pos_y = 0x50;
        set_sequence_from_table_5(o, 1, data_8002bde4_slot28);
        data_80051898_slot28.p = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_2_slot28(o);
        o->ot_depth = 3;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->pos_x = 0xe0;
        o->image_row = 0;
        o->pos_y = 0x80;
        set_sequence_from_table_5(o, 2, data_8002bde4_slot28);
        data_8005189c_slot28.p = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = one;
        o->field_02 = 0xa6;
        o->table_index = 0;
        data_800518a0_slot28.p = o;
    }
    set_menu_command_one_and_wait_pending(0x206);
    start_fade_mode_c01_and_wait();
}
