/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8003b504_slot28[];
extern SequenceStep *data_8003b50c_slot28[];
extern SequenceStep *data_8003b514_slot28[];
extern Object *data_80051a18_slot28[];
extern ObjectRef data_80051a40_slot28;
extern Object *data_80051a44_slot28[];
void set_sequence_of_fourth_hud_object_8_slot28(Object *obj, int arg);
void init_object_data_pointers_a_8_slot28(Object *obj);
void release_active_objects_in_table_6_slot28(void);

void advance_slot28_scene_spawn_one_object_slot28(Object *obj) {
    Object *p;
    SequenceStep **t;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x1e0;
        data_8018f5a0->pass_index += 1;
        release_active_objects_in_table_6_slot28();
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_8_slot28(p);
            t = data_8003b514_slot28;
            p->pos_x = 0xc8;
            p->pos_y = 0x90;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 8;
            set_sequence_from_table_5(p, 2, t);
            data_80051a18_slot28[0] = p;
            ((Slot28Obj *)p)->sequence_ptr = t;
        }
        set_sequence_from_table_5(data_80051a40_slot28.p, 0, data_8003b504_slot28);
        p = data_80051a44_slot28[0];
        p->pos_y = -0x40;
        p->field_01 = 1;
        p->ot_depth = 4;
        set_sequence_from_table_5(p, 0, data_8003b50c_slot28);
        set_sequence_of_fourth_hud_object_8_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
