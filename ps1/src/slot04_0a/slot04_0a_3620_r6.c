/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void apply_velocity_and_acceleration_slot04_0a(Object *object);

void clear_vel_y_start_sequence_with_meter_gain_slot04_0a(Object *obj) {
    obj->state_3++;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->vel_y = 0;
    add_to_meter_clamped(obj, 4);
    set_sequence_by_side_and_mode(obj, 0x53);
}

void update_move_and_hit_other_on_flag_1_slot04_0a(Object *obj) {
    int v;
    if ((u8)obj->step_flags != 2) {
        apply_velocity_and_acceleration_slot04_0a(obj);
    }
    if ((u8)obj->step_flags == 1) {
        play_side_sound_at(obj, obj->side, 8);
        obj->state_3++;
        if (obj->other->field_15b != 0) {
            set_other_hit_reaction_state_0(obj, 0x24, 0xd, 0x11, 0, 0, 0);
        } else {
            set_other_hit_reaction_state_0(obj, 0x24, 0xa, 0x11, 0, 0, 0);
        }
        if (obj->facing_side != 0) {
            obj->vel_x = 0xffff0000;
            v = 0x500;
        } else {
            obj->vel_x = 0x10000;
            v = -0x500;
        }
        obj->accel_x = v;
        obj->vel_y = 0;
    }
    update_sequence(obj);
}

void move_until_ground_then_land_slot04_0a(Object *obj) {
    apply_velocity_and_acceleration_slot04_0a(obj);
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        land_reset_fields_and_choose_next_state(obj);
    }
}
