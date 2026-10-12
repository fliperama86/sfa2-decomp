/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c2f2c_slot04_0b[])(Object *, Object *);
void follow_parent_frame_param_with_x_offset_slot04_0b(Object *obj, Object *parent);

void call_handler_by_state_0_with_parent_slot04_0b(Object *obj) {
    data_801c2f2c_slot04_0b[obj->state_0](obj, obj->parent);
}

void init_child_from_parent_fields_slot04_0b(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    follow_parent_frame_param_with_x_offset_slot04_0b(obj, parent);
}
