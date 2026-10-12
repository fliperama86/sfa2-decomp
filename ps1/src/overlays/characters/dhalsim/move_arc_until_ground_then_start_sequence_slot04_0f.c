/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_arc_until_ground_then_start_sequence_slot04_0f(Object *obj) {
    if (obj->vel_y >= 0) {
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->ground_y <= obj->pos_y) {
            *(s32 *)&obj->pos_x_frac += obj->vel_x;
            obj->vel_x += obj->accel_x;
            return;
        }
    }
    obj->state_3++;
    obj->done_flag = 0;
    obj->once_flag_2 = 0;
    obj->flag_25a_set = 0;
    play_side_sound_at(obj, obj->side, 0xe);
    set_sequence_by_side_and_mode(obj, 0x47);
}
