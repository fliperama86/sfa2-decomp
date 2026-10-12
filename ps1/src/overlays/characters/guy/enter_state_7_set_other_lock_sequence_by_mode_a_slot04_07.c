/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void enter_state_7_set_other_lock_sequence_by_mode_a_slot04_07(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    obj->field_157 = 0;
    obj->other->field_6b = 0x12;
    obj->field_6b = 0;
    obj->other_lock = 0x16;
    set_sequence_by_side_and_mode(obj, 0x50);
}
