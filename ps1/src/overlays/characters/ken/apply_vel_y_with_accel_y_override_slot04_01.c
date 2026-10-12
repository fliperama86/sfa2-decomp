/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void play_side_sound_from_step_flags_slot04_01(Object *obj);
extern ObjectFn data_801bef84_slot04_01[];

void apply_vel_y_with_accel_y_override_slot04_01(Object *obj) {
    if (obj->field_49 != 0 && obj->vel_y < 0) {
        obj->accel_y = 0xffff0000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}

void call_state_3_table_then_play_step_flags_sound_3_slot04_01(Object *obj) {
    data_801bef84_slot04_01[obj->state_3](obj);
    play_side_sound_from_step_flags_slot04_01(obj);
}
