/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c3e9c_slot04_08;
extern SequenceStep **data_801c3ea0_slot04_08;
void follow_other_frame_param_sequence_slot04_08(Object *obj, Object *other);

void init_follower_from_other_slot04_08(Object *obj, Object *other) {
    obj->state_0++;
    obj->field_1c = other->field_1c;
    obj->table_index = other->kind;
    obj->field_0c = other->field_0c;
    obj->image_row = other->image_row;
    obj->field_0e = other->field_0e;
    obj->variant_index = 0;
    if (other->side == 0) {
        data_801c3e9c_slot04_08 = data_1f8000b4;
    } else {
        data_801c3ea0_slot04_08 = data_1f800164;
    }
    follow_other_frame_param_sequence_slot04_08(obj, other);
}
