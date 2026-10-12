/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_airborne_until_ground_landing_2_slot04_00(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->active_state = 1;
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->state_3 = obj->state_3 + 1;
            obj->active_state = 0;
            obj->active_flag_3 = 0;
            obj->pos_y = obj->ground_y;
            set_sequence_by_side_and_mode(obj, 0x30);
            return;
        }
    }
    update_sequence(obj);
}
