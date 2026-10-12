/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c4670_slot04_04[];
void update_follower_by_parent_kind_slot04_04(Object *obj);
void follow_parent_frame_param_sequence_a_slot04_04(Object *obj, Object *p);
void follow_parent_frame_variant_sequence_slot04_04(Object *obj, Object *p);
void increment_state_0_slot04_04(Object *obj);

void init_follower_from_parent_kind_slot04_04(Object *obj) {
    Object *p = obj->parent;

    obj->state_0 = obj->state_0 + 1;
    obj->field_1c = p->field_1c;
    obj->state_3 = p->kind;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->field_0e = p->field_0e;
    obj->variant_index = 0;
    if (obj->table_index == 0) {
        if (p->side == 0) {
            data_801c4670_slot04_04[0] = data_1f8000b4;
        } else {
            data_801c4670_slot04_04[1] = data_1f800164;
        }
    } else {
        if (p->side == 0) {
            data_801c4670_slot04_04[2] = data_1f8000f0;
        } else {
            data_801c4670_slot04_04[3] = data_1f8001a0;
        }
    }
    update_follower_by_parent_kind_slot04_04(obj);
}

void update_follower_by_parent_kind_slot04_04(Object *obj) {
    Object *p = obj->parent;

    obj->field_01 = 0;
    if (p->kind != obj->state_3) {
        increment_state_0_slot04_04(obj);
    } else if (obj->table_index == 0) {
        follow_parent_frame_param_sequence_a_slot04_04(obj, p);
    } else {
        follow_parent_frame_variant_sequence_slot04_04(obj, p);
    }
}
