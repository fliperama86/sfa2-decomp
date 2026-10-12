/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void play_side_sound_from_step_flags_slot04_01(Object *obj);
extern ObjectFn data_801bf128_slot04_01[];

int move_by_velocity_return_falling_slot04_01(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->vel_y < 0;
}

void call_state_3_table_then_play_step_flags_sound_5_slot04_01(Object *obj) {
    data_801bf128_slot04_01[obj->state_3](obj);
    play_side_sound_from_step_flags_slot04_01(obj);
}
