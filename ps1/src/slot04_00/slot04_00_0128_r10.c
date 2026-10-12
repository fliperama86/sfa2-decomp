/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_states_and_other_lock_sequence_1a_b_slot04_00(Object *obj);

int func_801b0dd0_slot04_00(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        set_states_and_other_lock_sequence_1a_b_slot04_00(obj);
    }
    return r;
}
