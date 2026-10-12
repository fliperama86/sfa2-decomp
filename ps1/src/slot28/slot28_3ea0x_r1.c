/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051878_slot28;
extern Object *data_8005187c_slot28[];
extern ObjectRef data_8005188c_slot28;
extern SequenceStep *data_80029ecc_slot28[];
extern SequenceStep *data_80029ed0_slot28[];
extern SequenceStep *data_80029ed8_slot28[];
extern SequenceStep *data_80029ee0_slot28[];
extern SequenceStep *data_80029ee4_slot28[];
extern SequenceStep *data_80029ef4_slot28[];
void init_object_data_pointers_a_slot28(Object *obj);

void func_80013ea0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *o;
    int one;
    h->step_index_2 = h->step_index_2 + 1;
    h->countdown_timer = 0x258;
    start_fade_mode_1_and_wait();
    one = 1;
    data_80190568 = one;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_slot28(o);
        data_80051878_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 0;
        set_sequence_from_table_5(o, 1, data_80029ed0_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_slot28(o);
        data_8005188c_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 0;
        set_sequence_from_table_5(o, 0, data_80029ecc_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_slot28(o);
        data_8005187c_slot28[0] = o;
        o->pos_x = 0x60;
        o->pos_y = 0x20;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 2;
        set_sequence_from_table_5(o, 0, data_80029ed8_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_slot28(o);
        data_8005187c_slot28[1] = o;
        o->pos_x = 0x60;
        o->pos_y = 0x20;
        o->spawn_x = 0x20;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 3;
        set_sequence_from_table_5(o, 0, data_80029ee0_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_slot28(o);
        data_8005187c_slot28[2] = o;
        o->pos_x = 0xc0;
        o->pos_y = 0xa0;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_80029ee4_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_slot28(o);
        data_8005187c_slot28[3] = o;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x28;
        o->spawn_x = 0;
        o->image_row = 0;
        o->table_index = one;
        o->ot_depth = 0;
        o->vel_y = 0xb8;
        set_sequence_from_table_5(o, 0, data_80029ef4_slot28);
    }
    set_menu_command_one_and_wait_pending(0x300);
    start_fade_mode_c01_and_wait();
}
