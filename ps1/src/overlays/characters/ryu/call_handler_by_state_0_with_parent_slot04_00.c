/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c0574_slot04_00[])(Object *, Object *);
void follow_parent_frame_param_sequence_slot04_00(Object *obj, Object *parent);

void call_handler_by_state_0_with_parent_slot04_00(Object *obj) {
    data_801c0574_slot04_00[obj->state_0](obj, obj->parent);
}

void init_child_from_parent_fields_slot04_00(Object *obj, Object *parent) {
    obj->state_0++;
    obj->field_1c = parent->field_1c;
    obj->table_index = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->variant_index = 0;
    follow_parent_frame_param_sequence_slot04_00(obj, parent);
}

void follow_parent_frame_param_sequence_slot04_00(Object *obj, Object *parent) {
    int idx;
    int old;

    obj->field_01 = 0;
    if (obj->table_index != parent->kind) {
        obj->state_0++;
        return;
    }
    idx = parent->frame->frame_param;
    if (idx == 0) {
        obj->variant_index = 0;
        return;
    }
    old = obj->variant_index;
    obj->pos_x = parent->pos_x;
    obj->pos_y = parent->pos_y;
    obj->facing_side = parent->facing_side;
    obj->field_01 = 1;
    if (old == idx) {
        advance_sequence_checked(obj);
        return;
    }
    obj->variant_index = idx;
    if (parent->side == 0) {
        set_sequence_from_table_5(obj, idx, seqs_b4_left);
    } else {
        set_sequence_from_table_5(obj, idx, seqs_164_right);
    }
}
