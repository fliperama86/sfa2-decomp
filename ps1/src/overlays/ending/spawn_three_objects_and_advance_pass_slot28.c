/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051ccc_slot28[];
extern ObjectRef data_80051cac_slot28;
extern ObjectRef data_80051cb0_slot28;
extern ObjectRef data_80051cc0_slot28;
extern SequenceStep *data_800502c8_slot28[];
extern SequenceStep *data_800502e8_slot28[];
void init_object_data_pointers_a_18_slot28(Object *o);
void set_sequence_of_fourth_hud_object_18_slot28(Object *o, int arg);

void spawn_three_objects_and_advance_pass_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        screen_state->pass_index++;
        set_sequence_from_table_5(data_80051ccc_slot28[0], 1, data_800502c8_slot28);
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_18_slot28(p);
            p->pos_x = 0x170;
            p->pos_y = 0x70;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 4;
            set_sequence_from_table_5(p, 0, data_800502e8_slot28);
            data_80051cac_slot28.p = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_01 = 1;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_object_data_pointers_a_18_slot28(p);
            p->pos_x = 0x170;
            p->pos_y = 0x70;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 1, data_800502e8_slot28);
            data_80051cb0_slot28.p = p;
            p->box_tables = (BoxTables *)data_800502e8_slot28;
            p->field_01 = 1;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xa6;
            p->table_index = 0xc;
            data_80051cc0_slot28.p = p;
        }
        set_sequence_of_fourth_hud_object_18_slot28(obj, 1);
        start_fade_mode_c01_and_wait();
    }
}
