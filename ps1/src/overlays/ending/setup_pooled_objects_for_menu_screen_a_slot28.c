/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_8002fc8c_slot28[];
extern SequenceStep *data_8002fc90_slot28[];
extern SequenceStep *data_8002fca0_slot28[];
extern SequenceStep *data_8002fca8_slot28[];
extern SequenceStep *data_8002fce8_slot28[];
extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
extern ObjectRef data_800518f8_slot28;
extern Object *data_800518fc_slot28[];
extern ObjectRef data_80051908_slot28;
extern Object *data_8005190c_slot28[];
extern Object *data_800518d0_slot28[];
extern Object *data_800518d4_slot28[];
extern Object *data_800518d8_slot28[];
extern Object *data_800518dc_slot28[];
void init_object_data_pointers_a_3_slot28(Object *obj);
void clear_ten_entry_table_slot28(void);

void setup_pooled_objects_for_menu_screen_a_slot28(Object *obj) {
    Object *o;
    int one;
    screen_state->countdown_timer = 0x258;
    screen_state->step_index_2 = screen_state->step_index_2 + 1;
    clear_ten_entry_table_slot28();
    start_fade_mode_1_and_wait();
    one = 1;
    kind_handlers_enabled = one;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_3_slot28(o);
        data_800518f8_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 2, data_8002fc90_slot28);
        o->field_01 = one;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_3_slot28(o);
        data_8005190c_slot28[0] = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = one;
        set_sequence_from_table_5(o, 0, data_8002fc8c_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_3_slot28(o);
        data_800518fc_slot28[0] = o;
        o->pos_x = 0x40;
        o->pos_y = 0x20;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 5;
        set_sequence_from_table_5(o, 0, data_8002fca0_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        init_object_data_pointers_a_3_slot28(o);
        data_80051908_slot28.p = o;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x28;
        o->spawn_x = 0;
        o->image_row = 0;
        o->table_index = one;
        o->vel_y = 0xb8;
        o->ot_depth = 0;
        set_sequence_from_table_5(o, 0, data_8002fce8_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        Slot28Obj *p = (Slot28Obj *)obj;
        o->field_02 = 0x72;
        o->table_index = 4;
        o->ot_depth = 2;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->pos_x = 0xf8;
        o->pos_y = 0xa0;
        o->field_90 = (void *)0x80060000;
        o->data_ptr_a = data_8002e1d4_slot28;
        o->active_count = one;
        o->field_01 = one;
        o->image_row = 0;
        o->data_ptr_b = data_8002e758_slot28;
        if ((p->state_index & 0x7f) == 4) {
            set_sequence_from_table_5(o, 0, data_8002fca8_slot28);
        } else {
            set_sequence_from_table_5(o, 7, data_8002fca8_slot28);
        }
        data_800518d0_slot28[0] = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x72;
        o->ot_depth = 3;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x88;
        o->pos_y = 0x80;
        o->vel_y = 0x8000;
        o->accel_y = -0x400;
        o->field_90 = (void *)0x80060000;
        o->data_ptr_a = data_8002e1d4_slot28;
        o->table_index = 0;
        o->field_01 = 1;
        o->image_row = 0;
        o->data_ptr_b = data_8002e758_slot28;
        set_sequence_from_table_5(o, 1, data_8002fca8_slot28);
        data_800518d4_slot28[0] = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x72;
        o->ot_depth = 4;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x90;
        o->pos_y = 0x78;
        o->vel_y = 0xc000;
        o->accel_y = -0x800;
        o->field_90 = (void *)0x80060000;
        o->data_ptr_a = data_8002e1d4_slot28;
        o->table_index = 1;
        o->field_01 = 1;
        o->image_row = 0;
        o->data_ptr_b = data_8002e758_slot28;
        set_sequence_from_table_5(o, 5, data_8002fca8_slot28);
        data_800518d8_slot28[0] = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0xa6;
        o->table_index = 1;
        data_800518dc_slot28[0] = o;
    }
    set_menu_command_one_and_wait_pending(0x304);
    start_fade_mode_c01_and_wait();
}
