/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The same form as move_x_decel_advance_state_3_at_stop_slot04_08, whose unit has the measured
   variants. The two functions differ in their addresses only. */
void move_x_decel_advance_state_3_at_stop_2_slot04_08(Object *obj) {
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
