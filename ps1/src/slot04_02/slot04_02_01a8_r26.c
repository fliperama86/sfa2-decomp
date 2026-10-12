/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3074_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void update_fall_until_landing_or_step_flag_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y < obj->ground_y) {
        if ((u8)obj->step_flags != 0) {
            return;
        }
    } else {
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
    }
    update_sequence(obj);
}
