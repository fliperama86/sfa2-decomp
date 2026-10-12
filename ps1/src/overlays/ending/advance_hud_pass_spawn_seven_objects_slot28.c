/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051890_slot28;
extern SequenceStep *data_8002bde4_slot28[];
void init_object_data_pointers_and_spawn_b_slot28(Object *obj);
void release_two_stored_objects_slot28(Object *obj);
void set_sequence_of_fourth_hud_object_2_slot28(Object *obj, int arg);

void advance_hud_pass_spawn_seven_objects_slot28(Object *obj) {
    HudState *h;
    Object *p;
    Object *b;
    s16 t;
    if (obj->field_f0 == 0) {
        t = 0x3c0;
        data_8018f5a0->countdown_timer = t;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        release_two_stored_objects_slot28(obj);
        p = (Object *)pop_block_from_pool();
        b = p;
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x76;
            b->pos_x = 0xf0;
            b->pos_y = 0x70;
            b->ot_depth = 2;
            b->table_index = 1;
            b->field_0c = 0;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            init_object_data_pointers_and_spawn_b_slot28(b);
        }
        p = (Object *)pop_block_from_pool();
        b = p;
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x76;
            b->table_index = 2;
            b->pos_x = 0x90;
            b->pos_y = 0x68;
            b->field_0c = 0;
            b->ot_depth = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            init_object_data_pointers_and_spawn_b_slot28(b);
        }
        p = (Object *)pop_block_from_pool();
        b = p;
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x76;
            b->table_index = 2;
            b->pos_x = 0xa0;
            b->pos_y = 0x50;
            b->field_0c = 0;
            b->ot_depth = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            init_object_data_pointers_and_spawn_b_slot28(b);
        }
        p = (Object *)pop_block_from_pool();
        b = p;
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x76;
            b->table_index = 2;
            b->pos_x = 0x58;
            b->pos_y = 0x58;
            b->field_0c = 0;
            b->ot_depth = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            init_object_data_pointers_and_spawn_b_slot28(b);
        }
        p = (Object *)pop_block_from_pool();
        b = p;
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x76;
            b->table_index = 2;
            b->pos_x = 0x70;
            b->pos_y = 0x60;
            b->field_0c = 0;
            b->ot_depth = 1;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            init_object_data_pointers_and_spawn_b_slot28(b);
        }
        p = (Object *)pop_block_from_pool();
        b = p;
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x76;
            b->table_index = 3;
            b->pos_x = 0x89;
            b->pos_y = 0xa0;
            b->field_0c = 0;
            b->ot_depth = 0;
            b->box_tables = (BoxTables *)data_8002bde4_slot28;
            init_object_data_pointers_and_spawn_b_slot28(b);
            data_80051890_slot28.p = b;
        }
        p = (Object *)pop_block_from_pool();
        b = p;
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0xa5;
            b->pos_x = 0xf0;
            b->pos_y = 0x70;
            b->table_index = 0;
            b->field_0c = 0;
            b->ot_depth = 3;
            b->field_01 = 1;
            init_object_data_pointers_and_spawn_b_slot28(b);
            set_sequence_from_table_5(b, 6, data_8002bde4_slot28);
        }
        t = 2;
        set_sequence_of_fourth_hud_object_2_slot28(obj, t);
        start_fade_mode_1a01_and_wait();
    }
}
