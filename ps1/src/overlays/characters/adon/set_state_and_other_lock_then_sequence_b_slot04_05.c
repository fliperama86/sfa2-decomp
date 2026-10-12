/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_state_and_other_lock_then_sequence_b_slot04_05(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->field_15a = 8;
    obj->direction_index = 4;
    obj->facing_side = t;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x11;
    obj->other_lock = 0x15;
    set_sequence_by_side_and_mode(obj, 0x33);
}
