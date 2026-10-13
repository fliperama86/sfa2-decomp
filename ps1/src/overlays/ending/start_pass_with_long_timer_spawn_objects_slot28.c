/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051938_slot28;
extern Object *data_8005193c_slot28[];
extern ObjectRef data_80051918_slot28;
extern ObjectRef data_80051914_slot28;
extern SequenceStep *data_80031c90_slot28[];
extern SequenceStep *data_80031ca0_slot28[];
extern u8 data_800306d4_slot28[];
extern u8 data_80030a4c_slot28[];
void release_active_objects_in_table_2_slot28(void);
void set_sequence_of_fourth_hud_object_4_slot28(Object *obj, int arg);

void start_pass_with_long_timer_spawn_objects_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        screen_state->countdown_timer = 0x438;
        screen_state->pass_index++;
        release_active_objects_in_table_2_slot28();
        set_sequence_from_table_5(data_80051938_slot28.p, 1, data_80031c90_slot28);
        p = data_8005193c_slot28[0];
        p->pos_y = -0x50;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xa6;
            p->table_index = 5;
            p->field_01 = 0;
            data_80051918_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x34;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_800306d4_slot28;
            p->data_ptr_b = data_80030a4c_slot28;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->box_tables = (BoxTables *)data_80031ca0_slot28;
            p->ot_depth = 4;
            p->pos_x = 0x98;
            p->table_index = 0;
            p->field_01 = 1;
            p->image_row = 0;
            p->pos_y = 0x20;
            data_80051914_slot28.p = p;
        }
        set_sequence_of_fourth_hud_object_4_slot28(obj, 2);
        start_fade_mode_1a01_and_wait();
    }
}
