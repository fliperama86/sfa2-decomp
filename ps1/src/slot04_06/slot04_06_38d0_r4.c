/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_until_step_index_expires_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int v;

    obj->step_index -= 1;
    if (obj->step_index & 0x80) {
        o->state_3++;
        set_sequence_by_side_and_mode(o, 0x33);
    } else {
        v = 0x80000;
        if (o->facing_side == 0) {
            v = -0x80000;
        }
        *(s32 *)&o->pos_x_frac = v + *(s32 *)&o->pos_x_frac;
        update_sequence(o);
    }
}
