/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_new_vel_y_slot04_07(Object *obj);

void update_airborne_motion_until_ground_slot04_07(Object *obj) {
    int limit;

    if (move_by_velocity_return_new_vel_y_slot04_07(obj) >= 0) {
        update_sequence(obj);
    } else {
        limit = obj->ground_y;
        if (limit > obj->pos_y) {
            update_sequence(obj);
        } else {
            obj->state_3 = 1;
            obj->pos_y = limit;
            obj->pos_y_frac = 0;
            obj->active_state = 0;
            play_object_sound_by_kind(obj);
            set_sequence_by_side_and_mode(obj, 0x2e);
        }
    }
}
