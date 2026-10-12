/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void enter_state_2_seven_lock_other_with_saved_facing_slot04_03(Object *obj);

int try_spend_meter_then_start_move_slot04_03(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        enter_state_2_seven_lock_other_with_saved_facing_slot04_03(obj);
    }
    return r;
}
