/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: RESIDUAL set_sequence_by_other_state: original keeps the two "state_3 = 0" arms apart (no cross-jump merge); built merges them (-8 bytes). 3 source shapes tried. */



void call_handler_by_side_and_mode(Object *object) {
    ObjectFn fn;
    count_down_countdown_4_2(object);
    object->input_matched_flag = 0;
    if (object->field_02 == 0) {
        if (object->mode_flag_2 == 0) {
            fn = scratch_fn_e_left;
        } else {
            fn = scratch_call_left;
        }
    } else {
        if (object->mode_flag_2 == 0) {
            fn = scratch_fn_e_right;
        } else {
            fn = scratch_call_right;
        }
    }
    fn(object);
}

void call_state_3_handler_set_e(Object *object) {
    handler_table_19ec[object->state_3](object);
}
