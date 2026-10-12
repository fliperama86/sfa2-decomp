/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_new_vel_y_slot04_07(Object *obj);

void update_fall_until_ground_state_5_slot04_07(Object *obj) {
    int y;
    move_by_velocity_return_new_vel_y_slot04_07(obj);
    y = obj->ground_y;
    if (y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->pos_y = y;
        obj->state_3 = 5;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x3b);
    }
}
