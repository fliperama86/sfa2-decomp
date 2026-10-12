/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 try_start_meter_spend_action_state_5_slot04_02(Object *obj) {
    int r = 1;

    if (obj->mode_flag_2 == 0) {
        r = try_spend_meter_for_action(obj);
        if (!(u8)r) return 0;
    }
    set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 5);
    obj->active_flag_3 = 1;
    obj->other_lock = 0x19;
    obj->facing_side = obj->saved_facing;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->field_15a = 0;
    obj->other->field_6b = 0x15;
    set_sequence_by_side_and_mode(obj, 0x31);
    return r;
}
