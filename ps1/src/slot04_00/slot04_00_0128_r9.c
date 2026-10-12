/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0d60_slot04_00(Object *obj);

int func_801b0d10_slot04_00(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        func_801b0d60_slot04_00(obj);
    }
    return r;
}
