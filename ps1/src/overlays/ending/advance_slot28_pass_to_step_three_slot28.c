/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051ac0_slot28;
extern Object *data_80051ac4_slot28[];
extern Object *data_80051a98_slot28[];
extern SequenceStep *data_8003f084_slot28[];
extern SequenceStep *data_8003f090_slot28[];
extern SequenceStep *data_8003f09c_slot28[];
extern u8 data_8003dea0_slot28[];
extern u8 data_8003e230_slot28[];
void set_sequence_of_fourth_hud_object_10_slot28(Object *obj, int arg);
void init_object_data_pointers_a_10_slot28(Object *obj);
void release_active_objects_in_table_8_slot28(void);

void advance_slot28_pass_to_step_three_slot28(Object *obj) {
    HudState *h;
    Object *p;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 8;
        h->pass_index++;
        release_active_objects_in_table_8_slot28();
        set_sequence_from_table_5(data_80051ac0_slot28.p, 1, data_8003f084_slot28);
        set_sequence_from_table_5(data_80051ac4_slot28[0], 2, data_8003f090_slot28);
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_10_slot28(p);
            p->pos_x = 0xc8;
            p->pos_y = 0x90;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 3;
            set_sequence_from_table_5(p, 4, data_8003f09c_slot28);
            data_80051a98_slot28[0] = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x70;
            p->table_index = 2;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8003dea0_slot28;
            p->data_ptr_b = data_8003e230_slot28;
            p->pos_x = 0xac;
            p->pos_y = 0xaa;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->field_01 = 0;
            p->image_row = 0;
            p->ot_depth = 2;
            ((Slot28Obj *)p)->sequence_ptr = data_8003f09c_slot28;
            data_80051a98_slot28[1] = p;
        }
        set_sequence_of_fourth_hud_object_10_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
