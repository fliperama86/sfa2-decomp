/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_by_velocity_until_ground_then_reset_slot04_14(Object *obj);

void move_by_velocity_until_ground_then_reset_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    if (obj->ground_y <= (u16)obj->pos_y) {
        play_object_sound_by_kind(obj);
        obj->pos_y = (u16)obj->ground_y;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
