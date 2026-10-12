/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void follow_parent_frame_param_sequence_slot04_05(Object *obj, Object *parent);

void init_child_from_parent_fields_slot04_05(Object *obj, Object *p) {
    obj->state_0++;
    obj->field_1c = p->field_1c;
    obj->table_index = ((Slot04aObj *)p)->parent_side_copy;
    obj->field_0c = p->field_0c;
    obj->field_0e = p->field_0e;
    obj->variant_index = 0;
    obj->image_row = p->image_row;
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1e0;
    obj->field_90 = p->field_90;
    obj->data_ptr_a = p->data_ptr_a;
    obj->data_ptr_b = p->data_ptr_b;
    follow_parent_frame_param_sequence_slot04_05(obj, p);
}
