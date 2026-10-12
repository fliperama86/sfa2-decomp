/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c4670_slot04_04[];

void follow_parent_frame_param_sequence_a_slot04_04(Object *obj, Object *p) {
    u8 a;

    a = p->frame->frame_param;
    if (a == 0) {
        obj->variant_index = 0;
    } else {
        obj->pos_x = p->pos_x;
        obj->pos_y = p->pos_y;
        obj->facing_side = p->facing_side;
        obj->field_01 = 1;
        if (obj->variant_index == a) {
            advance_sequence_checked(obj);
        } else {
            obj->variant_index = a;
            set_sequence_entry(obj, data_801c4670_slot04_04[p->side][a]);
        }
    }
}

void follow_parent_frame_variant_sequence_slot04_04(Object *obj, Object *p) {
    u8 a;

    a = p->frame->field_0f;
    if (a == 0) {
        obj->variant_index = 0;
    } else {
        obj->pos_x = p->pos_x;
        obj->pos_y = p->pos_y;
        obj->facing_side = p->facing_side;
        obj->field_01 = 1;
        if (obj->variant_index == a) {
            advance_sequence_checked(obj);
        } else {
            obj->variant_index = a;
            set_sequence_entry(obj, data_801c4670_slot04_04[2 + p->side][a]);
        }
    }
}
