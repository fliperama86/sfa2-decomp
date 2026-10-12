/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051a80_slot28;
extern Object *data_80051a84_slot28[];
extern Object *data_80051a58_slot28[];
extern SequenceStep *data_8003d2d0_slot28[];
extern SequenceStep *data_8003d2d8_slot28[];
extern SequenceStep *data_8003d2e0_slot28[];
void copy_table_twice_to_buffer_slot28(Object *o);
void set_sequence_of_fourth_hud_object_9_slot28(Object *obj, int arg);
void init_object_data_pointers_a_9_slot28(Object *o);
void release_active_objects_in_table_7_slot28(void);

void advance_slot28_pass_to_step_two_slot28(Object *obj) {
    Object *p;
    HudState *g;
    if (obj->field_f0 == 0) {
        g = data_8018f5a0;
        g->countdown_timer = 0x258;
        g->pass_index = g->pass_index + 1;
        copy_table_twice_to_buffer_slot28(obj);
        release_active_objects_in_table_7_slot28();
        set_sequence_from_table_5(data_80051a80_slot28.p, 1, data_8003d2d0_slot28);
        p = data_80051a84_slot28[0];
        p->pos_x = 0x48;
        p->field_01 = 0;
        p->pos_y = 0x20;
        set_sequence_from_table_5(p, 1, data_8003d2d8_slot28);
        p->field_01 = 1;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_9_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0xa0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 4, data_8003d2e0_slot28);
            data_80051a58_slot28[0] = p;
        }
        start_fade_mode_1a01_and_wait();
        set_sequence_of_fourth_hud_object_9_slot28(obj, 2);
    }
}
