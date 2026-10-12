/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_by_velocity_return_new_vel_y_slot04_07(Object *obj);
void update_countdown_hit_check_or_end_slot04_07(Object *obj);
void start_arc_from_other_box_with_meter_and_sounds_slot04_07(Object *obj);
void start_fall_straight_down_slot04_07(Object *obj);

void update_airborne_motion_land_or_branch_slot04_07(Object *obj) {
    if (move_by_velocity_return_new_vel_y_slot04_07(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->state_3 = 5;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x3b);
    } else {
        if (obj->vel_y >= 0) {
            update_sequence(obj);
        } else if (obj->mode_flag_2 != 0) {
            update_countdown_hit_check_or_end_slot04_07(obj);
        } else if (!((obj->pressed_bits | obj->released_bits) & 0x94)) {
            update_sequence(obj);
        } else if (obj->other->bound_push_flag != 0 || obj->other->field_157 != 0 || (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x24, 0x18) == 0) {
            start_fall_straight_down_slot04_07(obj);
        } else {
            start_arc_from_other_box_with_meter_and_sounds_slot04_07(obj);
        }
    }
}
