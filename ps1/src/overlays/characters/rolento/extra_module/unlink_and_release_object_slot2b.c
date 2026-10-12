/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_8007ed88_slot2b[];
extern Object *data_8007ef28_slot2b;
extern Object *data_8007ef30_slot2b;
void copy_parent_pose_and_follow_frame_param_variant_slot2b(Object *o);

void unlink_and_release_object_slot2b(Object *o) {
    Object *p = data_8007ef28_slot2b;

    if ((s32)o == p->linked_object_3) {
        p->linked_object_3 = 0;
    }
    reset_and_push_block_object_type_8((Slab172 *)o);
}

void apply_velocity_and_acceleration_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;

    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
    obj->pos_y_hi -= o->vel_y;
    o->vel_y += o->accel_y;
}

void call_handler_by_state_0_with_parent_saved_slot2b(Object *o) {
    data_8007ef30_slot2b = o->parent;
    data_8007ed88_slot2b[o->state_0](o);
}

void init_object_from_linked_data_object_slot2b(Object *o) {
    Object *p;

    o->state_0++;
    p = data_8007ef30_slot2b;
    o->field_1c = p->field_1c;
    o->table_index = p->kind;
    o->field_0e = data_8007ef30_slot2b->field_0e;
    o->variant_index = 0;
    o->countdown = 0;
    copy_parent_pose_and_follow_frame_param_variant_slot2b(o);
}
