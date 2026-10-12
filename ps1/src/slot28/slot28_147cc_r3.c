/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051c4c_slot28;
extern Object *data_80051c50_slot28[];
extern Object *data_80051c24_slot28[];
extern ObjectRef data_80051c28_slot28;
extern ObjectRef data_80051c2c_slot28;
extern ObjectRef data_80051c30_slot28;
extern ObjectRef data_80051c34_slot28;
extern ObjectRef data_80051c38_slot28;
extern ObjectRef data_80051c3c_slot28;
extern SequenceStep *data_8004bd74_slot28[];
extern SequenceStep *data_8004bd7c_slot28[];
extern SequenceStep *data_8004bd84_slot28[];
extern u8 data_8004ab90_slot28[];
extern u8 data_8004aed8_slot28[];
void init_object_data_pointers_a_16_slot28(Object *o);
void release_active_objects_in_table_14_slot28(void);
void set_sequence_of_fourth_hud_object_16_slot28(Object *o, int arg);

void advance_pass_spawn_slot28_objects_d_slot28(Object *obj) {
    HudState *h;
    Object *p;
    int one;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 0x258;
        h->pass_index++;
        release_active_objects_in_table_14_slot28();
        set_sequence_from_table_5(data_80051c4c_slot28.p, 1, data_8004bd74_slot28);
        p = data_80051c50_slot28[0];
        p->pos_y = 0x20;
        set_sequence_from_table_5(p, 1, data_8004bd7c_slot28);
        one = 1;
        p->field_01 = one;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_16_slot28(p);
            p->pos_x = 0xb4;
            p->pos_y = 0x2d;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 2, data_8004bd84_slot28);
            data_80051c24_slot28[0] = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_16_slot28(p);
            p->pos_x = 0xbb;
            p->pos_y = 0x2f;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 3, data_8004bd84_slot28);
            data_80051c28_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_16_slot28(p);
            p->pos_x = 0xc8;
            p->pos_y = 0x3e;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 4, data_8004bd84_slot28);
            data_80051c2c_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_16_slot28(p);
            p->pos_x = 0xd9;
            p->pos_y = 0x4e;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 5, data_8004bd84_slot28);
            data_80051c30_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_16_slot28(p);
            p->pos_x = 0xf3;
            p->pos_y = 0x63;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 6, data_8004bd84_slot28);
            data_80051c34_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_16_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0xa0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 7, data_8004bd84_slot28);
            data_80051c38_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->field_02 = 0x1c;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8004ab90_slot28;
            p->data_ptr_b = data_8004aed8_slot28;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 4;
            p->active_count = one;
            p->table_index = 0;
            p->field_01 = one;
            p->image_row = 0;
            p->facing_side = 0;
            ((Slot28Obj *)p)->sequence_ptr = data_8004bd84_slot28;
            data_80051c3c_slot28.p = p;
        }
        set_sequence_of_fourth_hud_object_16_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
