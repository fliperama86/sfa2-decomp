/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);

void rise_until_vel_y_negative_then_flip_2_slot04_09(Object *obj) {
    if (obj->step_count != 0 && obj->other->active_state == 0) {
        obj->step_count = 0;
        game_state.state_param = 0x18;
    }
    apply_velocity_and_acceleration_slot04_09(obj);
    if (obj->vel_y < 0) {
        obj->state_3++;
        obj->facing_side ^= 1;
        set_sequence_by_side_and_mode(obj, 0x23);
    }
}

void apply_velocity_until_landing_2_slot04_09(Object *obj) {
    u16 t;

    if (obj->step_count != 0 && obj->other->active_state == 0) {
        obj->step_count = 0;
        game_state.state_param = 0x18;
    }
    apply_velocity_and_acceleration_slot04_09(obj);
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        t = *(u16 *)&obj->ground_y;
        obj->active_state = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = t;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x24);
    }
}
