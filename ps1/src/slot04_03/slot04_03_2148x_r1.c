/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_facing_with_decel_slot04_03(Object *obj);

void update_fall_until_ground_landing_slot04_03(Object *obj) {
    s16 y;

    if (*(u8 *)&obj->step_flags != 0) {
        update_sequence(obj);
        return;
    }
    obj->active_state = 1;
    move_x_by_facing_with_decel_slot04_03(obj);
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    y = obj->ground_y;
    if (y < obj->pos_y) {
        obj->pos_y = y;
        obj->active_state = 0;
        play_object_sound_by_kind(obj);
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x28);
    } else {
        update_sequence(obj);
    }
}
