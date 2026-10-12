/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8004a088_slot28[];
extern SequenceStep *data_8004a094_slot28[];
extern SequenceStep *data_8004a09c_slot28[];
extern u8 data_80048d70_slot28[];
extern u8 data_800490f4_slot28[];
extern Object *data_80051be4_slot28[];
extern ObjectRef data_80051c0c_slot28;
extern Object *data_80051c10_slot28[];
void func_80023c70_slot28(void);
void func_80023be0_slot28(Object *obj, int arg);

void advance_pass_spawn_two_slot28_objects_slot28(Object *obj) {
    Object *b;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        func_80023c70_slot28();
        set_sequence_from_table_5(data_80051c0c_slot28.p, 2, data_8004a088_slot28);
        b = data_80051c10_slot28[0];
        b->pos_x = 0x38;
        b->pos_y = 0x20;
        set_sequence_from_table_5(b, 1, data_8004a094_slot28);
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x1d;
            b->table_index = 3;
            b->ot_depth = 2;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->pos_x = 0x1a8;
            b->pos_y = 0xd5;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80048d70_slot28;
            b->data_ptr_b = data_800490f4_slot28;
            b->box_tables = (BoxTables *)data_8004a09c_slot28;
            b->field_01 = 1;
            b->image_row = 0;
            data_80051be4_slot28[0] = b;
            ((Slot28Obj *)b)->timer_reload = 0x20;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x1d;
            b->table_index = 2;
            b->ot_depth = 3;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->pos_x = -0x3d;
            b->pos_y = 0xc1;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80048d70_slot28;
            b->data_ptr_b = data_800490f4_slot28;
            b->box_tables = (BoxTables *)data_8004a09c_slot28;
            b->field_01 = 1;
            b->image_row = 0;
            data_80051be4_slot28[1] = b;
            ((Slot28Obj *)b)->timer_reload = 0x20;
        }
        func_80023be0_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
