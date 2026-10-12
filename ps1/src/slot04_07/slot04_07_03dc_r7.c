/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enter_state_7_set_other_lock_sequence_by_mode_b_slot04_07(Object *obj);

int try_spend_meter_then_start_move_2_slot04_07(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        enter_state_7_set_other_lock_sequence_by_mode_b_slot04_07(obj);
    }
    return r;
}
