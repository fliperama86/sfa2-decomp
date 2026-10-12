/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
void init_object_with_two_child_objects_slot27(Object *o);

void init_object_draw_fields_and_spawn_linked_slot27(Object *obj) {
    obj->ot_depth = 4;
    obj->data_ptr_a = data_80017c28_slot27;
    obj->data_ptr_b = data_8001aa14_slot27;
    obj->spawn_y = 0x1e0;
    obj->state_0++;
    obj->field_90 = (void *)0x80038000;
    obj->spawn_x = 0;
    obj->image_row = 0;
    obj->field_01 = 1;
    obj->facing_side = ref_other.p->facing_side;
    obj->vel_x = 0;
    obj->vel_y = 0;
    init_object_with_two_child_objects_slot27(obj);
}
