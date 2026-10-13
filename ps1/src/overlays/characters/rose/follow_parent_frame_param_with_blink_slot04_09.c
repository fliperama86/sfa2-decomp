/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_position_from_parent_table_offset_slot04_09(Object *obj, Object *parent, int i);

void follow_parent_frame_param_with_blink_slot04_09(Object *obj, Object *parent) {
    int n;
    int c;
    int d;
    SequenceStep **t;
    obj->field_01 = 0;
    if (obj->table_index != parent->kind) {
        obj->state_0++;
        return;
    }
    n = parent->frame->frame_param;
    if (n == 0) {
        obj->variant_index = 0;
        return;
    }
    set_position_from_parent_table_offset_slot04_09(obj, parent, n);
    obj->field_0c = parent->field_0c;
    obj->image_row = parent->image_row;
    obj->field_01 = 1;
    if (obj->variant_index == n) {
        d = obj->countdown - 1;
        obj->countdown = d;
        if ((s16)d < 0) {
            obj->countdown = 0;
            obj->field_0c = parent->field_0c;
            obj->image_row = parent->image_row;
            c = obj->countdown ^ 1;
            obj->countdown = c;
            if (c & 1) {
                obj->field_0c = 0xff;
                obj->image_row++;
            }
        }
        advance_sequence_checked(obj);
        if (((Slot04aObj *)obj)->mode == 1) {
            obj->facing_side = 0;
            obj->sequence_changed_flag = 1;
        }
    } else {
        obj->variant_index = n;
        if (parent->side == 0) {
            t = seqs_b4_left;
        } else {
            t = seqs_164_right;
        }
        set_sequence_from_table_5(obj, n, t);
        if (((Slot04aObj *)obj)->mode == 1) {
            obj->facing_side = 0;
        }
    }
}
