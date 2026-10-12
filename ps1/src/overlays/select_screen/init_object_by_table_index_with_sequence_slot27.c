/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_800284e0_slot27[];
extern GameState *data_800284f8_slot27;
void spawn_child_object_with_kind_index_slot27(Object *obj);
void set_origin_values_from_kind_index_tables_slot27(Object *obj);
void spawn_three_linked_objects_offset_x_slot27(Object *obj);

void init_object_by_table_index_with_sequence_slot27(Object *obj) {
    obj->ot_depth = 0x10;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_x = 0x20;
    obj->spawn_y = 0x1e0;
    obj->image_row = 0x18;
    obj->field_90 = (void *)0x80038000;
    obj->field_01 = 0;
    obj->state_0++;
    if (obj->table_index == 0) {
        set_origin_values_from_kind_index_tables_slot27(obj);
        spawn_three_linked_objects_offset_x_slot27(obj);
        set_sequence_from_table_5(obj, 0, data_800284e0_slot27);
    } else if (obj->table_index == 1) {
        set_origin_values_from_kind_index_tables_slot27(obj);
        obj->state_0 = 2;
        obj->pos_x = data_800284f8_slot27->field_d2;
        obj->pos_y = data_800284f8_slot27->field_d4;
        spawn_child_object_with_kind_index_slot27(obj);
        spawn_three_linked_objects_offset_x_slot27(obj);
        set_sequence_from_table_5(obj, 1, data_800284e0_slot27);
    } else if (obj->table_index >= 2) {
        obj->state_0 = 4;
        set_sequence_from_table_5(obj, obj->table_index, data_800284e0_slot27);
    }
}
