/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_hop_from_input_bits_slot04_05(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) != 0) {
        obj->facing_side = 1;
        obj->vel_x = -0x30000;
    } else {
        obj->vel_x = 0x30000;
    }
    obj->accel_x = 0;
    obj->vel_y = 0x40000;
    obj->accel_y = -0xe000;
    set_sequence_by_side_and_mode(obj, 0x19);
}

void advance_state_3_if_step_flags_byte_set_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
    }
    update_sequence(obj);
}

void move_back_until_step_flags_byte_two_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags == 2) {
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        update_sequence(obj);
    }
}

void update_air_motion_land_at_saved_pos_y_slot04_05(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
    }
    if (*(u8 *)&obj->step_flags != 3) {
        update_sequence(obj);
    }
}
