/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c08_slot04_03(Object *obj) {
    int d;

    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        d = -0x48;
        if (obj->facing_side != 0) {
            d = 0x48;
        }
        obj->pos_x = d + obj->pos_x;
    }
}
