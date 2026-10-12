/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void follow_parent_frame_param_with_x_offset_slot04_0b(Object *obj, Object *parent) {
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
    obj->pos_x = parent->pos_x;
    obj->pos_y = parent->pos_y;
    if (idx == 0x10) {
        if (obj->facing_side != 0) {
            obj->pos_x = obj->pos_x + 0x30;
        } else {
            obj->pos_x = obj->pos_x - 0x30;
        }
    }
    obj->facing_side = parent->facing_side;
    old = obj->variant_index;
    obj->field_01 = 1;
    if (old == idx) {
        advance_sequence_checked(obj);
        return;
    }
    obj->variant_index = idx;
    if (parent->side == 0) {
        set_sequence_from_table_5(obj, idx, data_1f8000b4);
    } else {
        set_sequence_from_table_5(obj, idx, data_1f800164);
    }
}
