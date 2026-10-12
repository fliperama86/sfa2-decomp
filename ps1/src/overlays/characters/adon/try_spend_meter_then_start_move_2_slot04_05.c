/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_state_and_other_lock_then_sequence_b_slot04_05(Object *obj);

int try_spend_meter_then_start_move_2_slot04_05(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        set_state_and_other_lock_then_sequence_b_slot04_05(obj);
    }
    return r;
}
