/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enter_state_7_action_3_set_other_field_slot04_16(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 3;
    obj->active_flag_3 = 1;
    obj->active_flag_4 = 1;
    obj->direction_index = 4;
    obj->other_lock = 0x1a;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->facing_side = obj->saved_facing;
    ref_other.p = obj->other;
    ref_other.p->field_6b = 0x16;
    set_sequence_by_side_and_mode(obj, 0x46);
}
