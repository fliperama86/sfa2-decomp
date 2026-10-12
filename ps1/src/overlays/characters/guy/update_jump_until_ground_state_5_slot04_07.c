/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_x_by_facing_with_decel_stop_slot04_07(Object *obj);

void update_jump_until_ground_state_5_slot04_07(Object *obj) {
    int t;

    t = (s16)obj->ground_y;
    if (t <= obj->pos_y) {
        obj->state_3 = 5;
        obj->pos_y = t;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x39);
    } else {
        if (*(u8 *)&obj->step_flags != 0) {
            update_sequence(obj);
            return;
        }
        move_x_by_facing_with_decel_stop_slot04_07(obj);
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
        obj->vel_y = obj->vel_y + obj->accel_y;
        update_sequence(obj);
    }
}
