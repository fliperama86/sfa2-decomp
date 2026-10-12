/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_8004384c_slot28[];
extern SequenceStep *data_80043858_slot28[];
extern SequenceStep *data_80043864_slot28[];
extern u8 data_8004231c_slot28[];
extern u8 data_800426b8_slot28[];
extern Object *data_80051b18_slot28[];
extern ObjectRef data_80051b40_slot28;
extern Object *data_80051b44_slot28[];
void release_active_objects_in_table_10_slot28(void);
void set_sequence_of_fourth_hud_object_12_slot28(Object *obj, int arg);

void advance_slot28_pass_spawn_object_b_slot28(Object *obj) {
    Object *p;
    int one;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x1a4;
        data_8018f5a0->pass_index++;
        release_active_objects_in_table_10_slot28();
        set_sequence_from_table_5(data_80051b40_slot28.p, 0, data_8004384c_slot28);
        p = data_80051b44_slot28[0];
        p->pos_y = -0x60;
        one = 1;
        p->field_01 = one;
        set_sequence_from_table_5(p, 0, data_80043858_slot28);
        set_sequence_of_fourth_hud_object_12_slot28(obj, 2);
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = one;
            p->field_02 = 0x2f;
            p->ot_depth = 2;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->pos_x = 0xb8;
            p->pos_y = 0x90;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8004231c_slot28;
            p->data_ptr_b = data_800426b8_slot28;
            p->table_index = one;
            p->field_01 = one;
            p->image_row = 0;
            ((Slot28Obj *)p)->sequence_ptr = data_80043864_slot28;
            data_80051b18_slot28[0] = p;
        }
        start_fade_mode_1a01_and_wait();
    }
}
