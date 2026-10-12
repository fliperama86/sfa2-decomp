/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_airborne_fall_until_ground_slot04_02(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        if (obj->pos_y >= obj->ground_y) {
            obj->active_state = 0;
            obj->pos_y = obj->ground_y;
            play_object_sound_by_kind(obj);
            reset_object_to_state_1_0_sequence_0(obj);
            return;
        }
    }
    update_sequence(obj);
}
