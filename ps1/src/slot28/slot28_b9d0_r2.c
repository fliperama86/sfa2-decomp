/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800519d8_slot28[];
extern ObjectRef data_80051a00_slot28;
extern Object *data_80051a04_slot28[];
extern SequenceStep *data_80039280_slot28[];
extern SequenceStep *data_80039290_slot28[];
extern SequenceStep *data_8003929c_slot28[];
extern u16 data_8003a04c_slot28[];
void set_sequence_of_fourth_hud_object_7_slot28(Object *obj, int arg);
void init_object_data_pointers_a_7_slot28(Object *obj);
void release_active_objects_in_table_5_slot28(void);

void advance_slot28_scene_spawn_eight_objects_slot28(Object *obj) {
    int i;
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        release_active_objects_in_table_5_slot28();
        set_sequence_from_table_5(data_80051a00_slot28.p, 1, data_80039280_slot28);
        p = data_80051a04_slot28[0];
        p->pos_x = 0x60;
        p->pos_y = -0x50;
        set_sequence_from_table_5(p, 0, data_80039290_slot28);
        for (i = 0; i < 7; i++) {
            p = (Object *)pop_block_from_pool();
            if (p != 0) {
                init_object_data_pointers_a_7_slot28(p);
                p->pos_x = data_8003a04c_slot28[i * 2];
                p->pos_y = data_8003a04c_slot28[i * 2 + 1];
                p->spawn_x = 0x60;
                p->spawn_y = 0x1e0;
                p->image_row = 0;
                p->ot_depth = 2;
                set_sequence_from_table_5(p, 3, data_8003929c_slot28);
                data_800519d8_slot28[i] = p;
            }
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_7_slot28(p);
            p->pos_x = 0xc0;
            p->pos_y = 0x81;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 4, data_8003929c_slot28);
            data_800519d8_slot28[7] = p;
        }
        set_sequence_of_fourth_hud_object_7_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}
