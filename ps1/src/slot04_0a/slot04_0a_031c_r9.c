/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_state_7_move_9_with_other_lock_slot04_0a(Object *obj);

int func_801b116c_slot04_0a(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_state_7_move_9_with_other_lock_slot04_0a(obj);
    }
    return r;
}
