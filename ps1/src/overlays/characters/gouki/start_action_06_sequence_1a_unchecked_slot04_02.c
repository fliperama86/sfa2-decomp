/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

int start_action_06_sequence_1a_unchecked_slot04_02(Object *obj) {
    set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->other_lock = 0x1e;
    obj->facing_side = obj->saved_facing;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->other->field_6b = 0x1a;
    set_sequence_by_side_and_mode(obj, 0x1a);
    return 0;
}
