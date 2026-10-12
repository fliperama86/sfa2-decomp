/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_arc_from_other_box_with_meter_and_sounds_slot04_07(Object *obj);
void start_fall_straight_down_slot04_07(Object *obj);

void update_countdown_hit_check_or_end_slot04_07(Object *obj) {
    if (obj->other->bound_push_flag == 0 && obj->other->field_157 == 0 && (u8)try_set_other_state_if_in_range(obj, -8, 0x24, -0x24, 0x18)) {
        start_arc_from_other_box_with_meter_and_sounds_slot04_07(obj);
    } else {
        obj->countdown = (s16)obj->countdown - 0x100;
        if ((s16)obj->countdown != 0) {
            update_sequence(obj);
        } else {
            start_fall_straight_down_slot04_07(obj);
        }
    }
}

void start_arc_from_other_box_with_meter_and_sounds_slot04_07(Object *obj) {
    obj->vel_y = 0x40000;
    obj->accel_y = -0x5800;
    obj->state_3 = obj->state_3 + 1;
    obj->accel_x = 0;
    if (obj->facing_side == 0) {
        obj->vel_x = -0x20000;
    } else {
        obj->vel_x = 0x20000;
    }
    add_to_meter_clamped(obj, 7);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 7);
    set_sequence_by_side_and_mode(obj, 0x21);
    set_position_from_other_box(obj);
}

void start_fall_straight_down_slot04_07(Object *obj) {
    obj->state_3 = 6;
    obj->accel_y = -0x5800;
    obj->vel_x = 0;
    obj->accel_x = 0;
    obj->vel_y = 0;
    obj->active_flag_3 = 1;
    set_sequence_by_side_and_mode(obj, 0x3a);
}
