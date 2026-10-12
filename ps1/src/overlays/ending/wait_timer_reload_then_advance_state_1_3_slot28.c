/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void wait_timer_reload_then_advance_state_1_3_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->timer_reload--;
    if (obj->timer_reload == 0) {
        o->state_1++;
    }
}
