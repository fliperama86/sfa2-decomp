/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_states_and_other_lock_then_sequence_c_slot04_01(Object *obj);

int try_spend_meter_then_start_move_2_slot04_01(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        set_states_and_other_lock_then_sequence_c_slot04_01(obj);
    }
    return r;
}
