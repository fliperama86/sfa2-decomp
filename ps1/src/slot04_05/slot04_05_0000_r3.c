/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_jump_flip_facing_near_edge_slot04_05(Object *obj) {
    int e;
    s16 d;
    int base;
    int k;

    k = -0x10000;
    obj->state_3 = obj->state_3 + 1;
    base = box_margin[0];
    if (obj->facing_side == 0) {
        base += 0x180;
    }
    e = base - (u16)obj->pos_x;
    d = e;
    if (obj->facing_side != 0) {
        d = -e;
    }
    if (d < 0x30) {
        obj->facing_side = obj->facing_side ^ 1;
    }
    if (obj->facing_side == 0) {
        k = 0x10000;
    }
    obj->vel_y = 0x50000;
    obj->accel_y = -0x4000;
    obj->vel_x = k;
    obj->accel_x = 0;
    obj->active_state = 1;
}

void advance_state_3_if_step_flags_byte_zero_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags == 0) {
        obj->state_3 = obj->state_3 + 1;
    }
    update_sequence(obj);
}

void update_air_motion_until_landing_step_2_slot04_05(Object *obj) {
    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
    }
    if (*(u8 *)&obj->step_flags != 2) {
        update_sequence(obj);
    }
}

void set_side_bit_on_sequence_flag_and_update_slot04_05(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        game_state.side_bits_2 |= 1 << obj->side;
    }
    update_sequence(obj);
}
