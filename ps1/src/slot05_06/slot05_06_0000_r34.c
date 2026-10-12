/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
void apply_vertical_velocity_slot05_06(Object *obj);

void update_air_motion_or_land_slot05_06(Object *obj) {
    apply_vertical_velocity_slot05_06(obj);
    if (obj->pos_y < obj->ground_y) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
    } else {
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void move_until_step_flag_then_play_sound_slot05_06(Object *obj) {
    Object *p = obj->other;

    if ((s16)obj->step_flags >= 0) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
        update_sequence(obj);
    } else {
        obj->state_3++;
        play_sound_at(p, p->side, 0x34e);
        set_sequence_by_side_and_mode(obj, 0x2e);
    }
}
