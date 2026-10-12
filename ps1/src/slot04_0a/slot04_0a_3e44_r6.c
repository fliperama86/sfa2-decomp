/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void follow_parent_frame_param_sequence_slot04_0a(Object *obj, Object *parent);

void init_object_from_parent_kind_slot04_0a(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    obj->vel_x = 0;
    follow_parent_frame_param_sequence_slot04_0a(obj, parent);
}

