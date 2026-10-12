/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_state_7_move_7_with_other_lock_slot04_08(Object *obj);

int try_spend_meter_and_start_move_7_slot04_08(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_state_7_move_7_with_other_lock_slot04_08(obj);
    }
    return r;
}
