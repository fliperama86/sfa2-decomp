/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1fc0_slot04_0b(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    update_sequence(o);
    if (obj->mode != 0) {
        o->active_state = 1;
        o->state_3++;
        if (o->facing_side == 0) {
            o->pos_x -= 0x28;
        } else {
            o->pos_x += 0x28;
        }
    }
}
