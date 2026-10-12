/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Object *data_801c575c_slot04_06;
extern ObjectFn data_801c55e4_slot04_06[];
void add_table_offset_to_position_slot04_06(Object *obj);
void set_sequence_from_table_index_by_sound_side_slot04_06(Object *obj);

void init_object_from_linked_and_call_table_handler_slot04_06(Object *obj) {
    Object *p;

    obj->state_0++;
    obj->field_0c = data_801c575c_slot04_06->field_0c;
    obj->image_row = data_801c575c_slot04_06->image_row;
    p = data_801c575c_slot04_06;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0e = p->field_0e;
    obj->ot_depth = 6;
    obj->facing_side = data_801c575c_slot04_06->facing_side;
    ((Slot04aObj *)obj)->saved_pos_y = ((Slot04aObj *)data_801c575c_slot04_06)->saved_pos_y;
    add_table_offset_to_position_slot04_06(obj);
    data_801c55e4_slot04_06[obj->table_index](obj);
    set_sequence_from_table_index_by_sound_side_slot04_06(obj);
}

void set_ot_depth_1_variant_index_0_slot04_06(Object *obj) {
    obj->ot_depth = 1;
    obj->variant_index = 0;
}

void set_ot_depth_and_variant_index_slot04_06(Object *obj) {
    obj->ot_depth = 1;
    obj->variant_index = 0x10;
}
