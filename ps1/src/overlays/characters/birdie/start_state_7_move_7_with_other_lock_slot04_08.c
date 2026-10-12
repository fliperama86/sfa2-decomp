/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_state_7_move_7_with_other_lock_slot04_08(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->field_15a = 7;
    obj->other_lock = 0x17;
    obj->facing_side = t;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x13;
    set_sequence_by_side_and_mode(obj, 0x5b);
}
