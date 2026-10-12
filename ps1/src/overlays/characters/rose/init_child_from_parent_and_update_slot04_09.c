/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void follow_parent_frame_param_with_blink_slot04_09(Object *obj, Object *parent);

void init_child_from_parent_and_update_slot04_09(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->image_row = parent->image_row;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    obj->countdown = 0;
    follow_parent_frame_param_with_blink_slot04_09(obj, parent);
}
