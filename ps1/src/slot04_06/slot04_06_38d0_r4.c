/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3cf8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int v;

    obj->step_index -= 1;
    if (obj->step_index & 0x80) {
        o->state_3++;
        func_801307e0(o, 0x33);
    } else {
        v = 0x80000;
        if (o->facing_side == 0) {
            v = -0x80000;
        }
        *(s32 *)&o->pos_x_frac = v + *(s32 *)&o->pos_x_frac;
        update_sequence(o);
    }
}
