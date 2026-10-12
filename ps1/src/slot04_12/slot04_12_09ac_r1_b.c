/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_spend_meter_for_action(Object *object);
/* functions of other units of this module */
void start_action_variant_9_lock_other_slot04_12(Object *obj);
u8 func_801b1328_slot04_12(Object *obj);

u8 func_801b1328_slot04_12(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_action_variant_9_lock_other_slot04_12(obj);
    }
    return r;
}
