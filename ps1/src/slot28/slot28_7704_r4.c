/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_child_object_from_source_slot28(Object *obj, Object *src);

void drop_object_then_spawn_child_objects_slot28(Object *obj) {
    Object *p;
    *(u32 *)&obj->pos_y_frac += 0x8000;
    if (obj->pos_y >= 0xa1) {
        obj->pos_y = 0xa0;
        obj->state_1++;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x34;
            p->pos_x = 0xf0;
            p->pos_y = 0x80;
            p->spawn_x = 0x60;
            p->table_index = 1;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 1;
            p->field_01 = 1;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            p->box_tables = obj->box_tables;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_child_object_from_source_slot28(p, obj);
            p->pos_x = 0xf0;
            p->pos_y = 0x80;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 2, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_child_object_from_source_slot28(p, obj);
            p->pos_x = 0xe0;
            p->pos_y = 0x50;
            p->ot_depth = 3;
            set_sequence_from_table_5(p, 4, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_child_object_from_source_slot28(p, obj);
            p->pos_x = 0xf0;
            p->pos_y = 0x90;
            p->ot_depth = 3;
            set_sequence_from_table_5(p, 5, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_child_object_from_source_slot28(p, obj);
            p->pos_x = 0x70;
            p->pos_y = 0x50;
            p->ot_depth = 3;
            set_sequence_from_table_5(p, 6, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            init_child_object_from_source_slot28(p, obj);
            p->pos_x = 0x70;
            p->pos_y = 0x80;
            p->ot_depth = 3;
            set_sequence_from_table_5(p, 7, (SequenceStep **)obj->box_tables);
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xa6;
            p->table_index = 6;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xa6;
            p->table_index = 7;
        }
    }
}
