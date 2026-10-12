/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051964_slot28;
extern Object *data_80051954_slot28[];
extern ObjectRef data_80051958_slot28;
extern ObjectRef data_8005195c_slot28;
extern u8 data_80032a64_slot28[];
extern u8 data_80032ec8_slot28[];
extern SequenceStep *data_80034530_slot28[];
void release_active_objects_in_table_3_slot28(void);
void set_sequence_of_fourth_hud_object_5_slot28(Object *obj, int arg);

void advance_hud_pass_when_object_flag_set_slot28(Object *obj) {
    Object *b = data_80051964_slot28.p;
    if (b->field_6b != 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index += 1;
        release_active_objects_in_table_3_slot28();
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            int one = 1;
            b->active_count = one;
            b->field_02 = 0x20;
            b->table_index = 8;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80032a64_slot28;
            b->data_ptr_b = data_80032ec8_slot28;
            b->pos_x = 0x118;
            b->pos_y = 0xa0;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->field_01 = one;
            b->image_row = 0;
            b->ot_depth = 2;
            set_sequence_from_table_5(b, 4, data_80034530_slot28);
            data_80051954_slot28[0] = b;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            int one = 1;
            b->active_count = one;
            b->field_02 = 0xa5;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80032a64_slot28;
            b->data_ptr_b = data_80032ec8_slot28;
            b->pos_x = 0xc0;
            b->pos_y = 0x60;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e1;
            b->table_index = 0;
            b->field_01 = one;
            b->image_row = 0;
            b->ot_depth = 4;
            set_sequence_from_table_5(b, 0xa, data_80034530_slot28);
            data_80051958_slot28.p = b;
        }
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            int one = 1;
            b->active_count = one;
            b->field_02 = 0xa5;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = data_80032a64_slot28;
            b->data_ptr_b = data_80032ec8_slot28;
            b->pos_x = 0xc0;
            b->pos_y = 0xa0;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e1;
            b->table_index = 0;
            b->field_01 = one;
            b->image_row = 0;
            b->ot_depth = 4;
            set_sequence_from_table_5(b, 0xa, data_80034530_slot28);
            data_8005195c_slot28.p = b;
        }
        start_fade_mode_1a01_and_wait();
        set_sequence_of_fourth_hud_object_5_slot28(obj, 2);
    }
}
