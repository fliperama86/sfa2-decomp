/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051b8c_slot28;
extern Object *data_80051b90_slot28[];
extern ObjectRef data_80051b9c_slot28;
extern Object *data_80051b64_slot28[];
extern SequenceStep *data_80045c40_slot28[];
extern SequenceStep *data_80045c4c_slot28[];
extern u8 data_80044620_slot28[];
extern u8 data_800449d0_slot28[];
extern SequenceStep *data_80045c58_slot28[];
void set_sequence_of_fourth_hud_object_13_slot28(Object *obj, int arg);
void release_active_objects_in_table_11_slot28(void);

void advance_slot28_pass_to_step_three_c_slot28(Object *obj) {
    Object *b;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        release_active_objects_in_table_11_slot28();
        set_sequence_from_table_5(data_80051b8c_slot28.p, 0, data_80045c40_slot28);
        b = data_80051b90_slot28[0];
        b->pos_y = -0x50;
        set_sequence_from_table_5(b, 1, data_80045c4c_slot28);
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x7a;
            b->ot_depth = 2;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->pos_x = 0xb8;
            b->pos_y = 0x90;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80044620_slot28;
            b->data_ptr_b = data_800449d0_slot28;
            b->table_index = 1;
            b->field_01 = 1;
            b->image_row = 0;
            b->box_tables = (BoxTables *)data_80045c58_slot28;
            data_80051b64_slot28[0] = b;
        }
        set_sequence_of_fourth_hud_object_13_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}

void advance_pass_when_sequence_ends_set_variant_slot28(Object *obj) {
    Object *p = data_80051b9c_slot28.p;
    if ((s16)p->step_flags < 0) {
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        p = data_80051b64_slot28[0];
        p->variant_index = 0xff;
        set_sequence_of_fourth_hud_object_13_slot28(obj, 4);
    }
}
