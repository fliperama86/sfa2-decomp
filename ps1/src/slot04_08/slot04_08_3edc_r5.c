/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c3e9c_slot04_08;
extern SequenceStep **data_801c3ea0_slot04_08;
void increment_state_0_slot04_08(Object *obj, Object *other);

void follow_other_frame_param_sequence_slot04_08(Object *obj, Object *other) {
    obj->field_01 = 0;
    if (other->kind == obj->table_index) {
        if (other->frame->frame_param != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&other->pos_x_frac;
            *(s32 *)&obj->pos_y_frac = *(s32 *)&other->pos_y_frac;
            obj->facing_side = other->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != other->frame->frame_param) {
                obj->variant_index = other->frame->frame_param;
                if (other->side == 0) {
                    set_sequence_from_table_5(obj, obj->variant_index, data_801c3e9c_slot04_08);
                } else {
                    set_sequence_from_table_5(obj, obj->variant_index, data_801c3ea0_slot04_08);
                }
            } else {
                advance_sequence_checked(obj);
            }
        } else {
            obj->variant_index = 0;
        }
    } else {
        increment_state_0_slot04_08(obj, other);
    }
}
