/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_move_type_8_with_other_lock_slot04_0b(Object *obj);

int spend_meter_start_move_type_eight_slot04_0b(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_move_type_8_with_other_lock_slot04_0b(obj);
    }
    return r;
}
