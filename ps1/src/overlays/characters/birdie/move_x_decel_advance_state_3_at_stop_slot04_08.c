/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* Each arm puts its comparison into a local and tests the local. With the
   comparisons tested in place, this function differs from the original in 4
   instruction slots; with the local in one arm only, in 4; with one test of
   the local after both arms, in 2, and it is 4 bytes longer. */
void move_x_decel_advance_state_3_at_stop_slot04_08(Object *obj) {
    int below;
    ((Slot04aObj *)obj)->field_1c8--;
    if (((Slot04aObj *)obj)->field_1c8 == 0) {
        obj->active_flag_4 = 0;
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (obj->facing_side == 0) {
        below = -obj->vel_x < 0;
        if (below) {
            obj->state_3++;
        }
    } else {
        below = obj->vel_x < 0;
        if (below) {
            obj->state_3++;
        }
    }
    update_sequence(obj);
}
