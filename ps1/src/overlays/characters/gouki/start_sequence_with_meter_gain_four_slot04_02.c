/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_with_meter_gain_four_slot04_02(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = 1;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    set_sequence_by_side_and_mode(obj, obj->field_49 != 0 ? 0x68 : 0x37);
}

void advance_to_directional_sequence_at_end_slot04_02(Object *obj) {
    u16 n = 0x32;

    if ((s16)obj->step_flags < 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_49 != 0) {
            n = 0x69;
        }
        n = (obj->direction_index >> 1) + n;
        set_sequence_by_side_and_mode(obj, n);
    } else {
        update_sequence(obj);
    }
}
