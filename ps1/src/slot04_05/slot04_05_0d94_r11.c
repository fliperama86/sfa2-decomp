/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_air_motion_until_landing_step_2_2_slot04_05(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
    }
    if (*(u8 *)&obj->step_flags != 2) {
        update_sequence(obj);
    }
}
