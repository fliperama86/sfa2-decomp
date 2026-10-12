/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_fall_and_land_second_variant(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object) != 0) {
        update_sequence(object);
    } else {
        object->state_3 = object->state_3 + 1;
        push_other_object_x_by_table(object);
        object->active_state = 0;
        object->countdown = 8;
        object->pos_y = object->ground_y;
        set_sequence_by_side_and_mode_2(object, 0x16);
        if ((s16)object->health >= 0) {
            play_kind_dependent_sound_by_a_and_b(object, 1, 0);
        } else {
            play_kind_dependent_sound_by_a_and_b(object, 1, 1);
        }
    }
}

void count_down_then_advance_or_enter_state_1_6(Object *object) {
    object->countdown = (s16)object->countdown - 1;
    if ((s16)object->countdown != 0) {
        update_sequence(object);
    } else if ((s16)object->health < 0) {
        enter_state_1_6_and_set_sequence(object);
    } else {
        object->state_3 = object->state_3 + 1;
        set_sequence_by_side_and_mode_2(object, 0x19);
    }
}
