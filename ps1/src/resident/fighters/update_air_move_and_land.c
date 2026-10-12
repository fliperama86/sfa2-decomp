/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_air_move_and_land(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object) == 0) {
        object->pos_y = (u16)object->ground_y;
        object->state_0 = 1;
        object->state_2 = 3;
        object->state_1 = 0;
        object->state_3 = 2;
        object->active_flag_3 = 0;
        object->active_state = 0;
        play_object_sound_by_kind(object);
        if (find_nearby_enemy_and_get_flag(object) != 0) {
            enter_state_2_6_by_button_input(object);
        } else {
            set_sequence_by_side(object, 0x11);
        }
    } else {
        if ((s16)object->step_flags < 0) {
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 3;
            object->state_3 = 1;
            object->active_flag_3 = 0;
        }
        update_sequence(object);
    }
}
