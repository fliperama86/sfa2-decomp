/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep **data_801c0aa8_slot04_03;
extern SequenceStep **data_801c0aac_slot04_03;

void follow_parent_frame_param_sequence_slot04_03(Object *obj, Object *p);

void init_follower_copy_parent_fields_and_follow_slot04_03(Object *obj, Object *p) {
    obj->state_0++;
    obj->field_1c = p->field_1c;
    obj->field_0c = p->field_0c;
    obj->image_row = p->image_row;
    obj->field_0e = p->field_0e;
    obj->variant_index = 0;
    if (p->side == 0) {
        data_801c0aa8_slot04_03 = *(SequenceStep ***)0x1f8000b4;
    } else {
        data_801c0aac_slot04_03 = *(SequenceStep ***)0x1f800164;
    }
    follow_parent_frame_param_sequence_slot04_03(obj, p);
}

void follow_parent_frame_param_sequence_slot04_03(Object *obj, Object *p) {
    obj->field_01 = 0;
    if (p->kind == obj->table_index) {
        if (p->frame->frame_param != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&p->pos_x_frac;
            *(s32 *)&obj->pos_y_frac = *(s32 *)&p->pos_y_frac;
            obj->facing_side = p->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != p->frame->frame_param) {
                obj->variant_index = p->frame->frame_param;
                if (p->side == 0) {
                    set_sequence_from_table_5(obj, obj->variant_index, data_801c0aa8_slot04_03);
                } else {
                    set_sequence_from_table_5(obj, obj->variant_index, data_801c0aac_slot04_03);
                }
            } else {
                advance_sequence_checked(obj);
            }
        } else {
            obj->variant_index = 0;
        }
    } else {
        obj->state_0++;
    }
}
