/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0f48_slot04_01(Object *obj);

int func_801b0ef8_slot04_01(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        func_801b0f48_slot04_01(obj);
    }
    return r;
}
