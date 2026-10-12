/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_spend_meter_for_action(Object *object);

int try_start_meter_action_with_other_lock_five_slot04_0f(Object *o) {
    if (!try_spend_meter_for_action(o)) return 0;
    o->state_0 = 1;
    o->state_1 = 0;
    o->state_3 = 0;
    o->field_157 = 0;
    o->field_6b = 0;
    o->state_2 = 7;
    o->active_flag_3 = 1;
    o->facing_side = o->saved_facing;
    o->field_15a = 5;
    o->other_lock = 0x19;
    o->other->field_6b = 0x15;
    set_sequence_by_side_and_mode(o, 0x48);
    return 1;
}
