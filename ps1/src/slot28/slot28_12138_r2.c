/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051bcc_slot28;
extern Object *data_80051bd0_slot28[];
extern ObjectRef data_80051bd4_slot28;
extern ObjectRef data_80051bdc_slot28;
extern ObjectRef data_80051be0_slot28;
extern Object *data_80051ba4_slot28[];
extern SequenceStep *data_80047bd4_slot28[];
extern SequenceStep *data_80047bd8_slot28[];
extern SequenceStep *data_80047bdc_slot28[];
extern SequenceStep *data_80047be0_slot28[];
extern SequenceStep *data_80047be4_slot28[];
extern SequenceStep *data_80047bf8_slot28[];
extern u8 data_800469ec_slot28[];
extern u8 data_80046d34_slot28[];
void func_80022ca8_slot28(void);
void func_80022c6c_slot28(Object *obj);

void init_slot28_scene_d_objects_slot28(Object *obj) {
    Object *o;
    data_8018f5a0->countdown_timer = 0x12c;
    data_8018f5a0->step_index_2 = data_8018f5a0->step_index_2 + 1;
    func_80151020(0x504);
    func_80022ca8_slot28();
    start_fade_mode_1_and_wait();
    data_80190568 = 1;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bcc_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 1;
        set_sequence_from_table_5(o, 0, data_80047bd8_slot28);
        o->field_01 = 1;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051be0_slot28.p = o;
        o->pos_x = -8;
        o->pos_y = -8;
        o->spawn_x = 0;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 1;
        set_sequence_from_table_5(o, 0, data_80047bd4_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bd0_slot28[0] = o;
        o->pos_x = 0x58;
        o->pos_y = 0x20;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1e0;
        o->image_row = 0;
        o->ot_depth = 4;
        o->field_01 = 0;
        set_sequence_from_table_5(o, 0, data_80047bdc_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bd4_slot28.p = o;
        o->pos_x = 0x58;
        o->pos_y = -0x60;
        o->spawn_x = 0x20;
        o->spawn_y = 0x1e0;
        o->field_01 = 1;
        o->image_row = 0;
        o->ot_depth = 5;
        set_sequence_from_table_5(o, 0, data_80047be0_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        func_80022c6c_slot28(o);
        data_80051bdc_slot28.p = o;
        o->spawn_y = 0x1e0;
        o->pos_x = 0x28;
        o->spawn_x = 0;
        o->image_row = 0;
        o->table_index = 1;
        o->vel_y = 0xb8;
        o->ot_depth = 0;
        set_sequence_from_table_5(o, 0, data_80047bf8_slot28);
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->field_02 = 0x65;
        o->ot_depth = 2;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->field_90 = (void *)0x80060000;
        o->data_ptr_a = data_800469ec_slot28;
        o->data_ptr_b = data_80046d34_slot28;
        o->active_count = 1;
        o->table_index = 0;
        o->field_01 = 1;
        o->image_row = 0;
        o->box_tables = (BoxTables *)data_80047be4_slot28;
        data_80051ba4_slot28[0] = o;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->field_02 = 0xa6;
        o->active_count = 1;
        o->table_index = 10;
        data_80051ba4_slot28[1] = o;
    }
    start_fade_mode_c01_and_wait();
}
