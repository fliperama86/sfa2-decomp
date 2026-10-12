/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int apply_velocity_and_check_ground(Object *object) {
    *(s32 *)&object->pos_x_frac += object->vel_x;
    object->vel_x += object->accel_x;
    *(u32 *)&object->pos_y_frac -= object->vel_y;
    object->vel_y += object->accel_y;
    return object->pos_y <= object->ground_y;
}

void apply_vel_x_and_check_input_bit(Object *object) {
    if (object->variant_index != 0) {
        if (object->input_bits_2 & 0x8000) start_move_state_1_0_2_1_with_sequence_by_input(object);
    } else {
        if (object->input_bits_2 & 0x2000) start_move_state_1_0_2_1_with_sequence_by_input(object);
    }
    *(s32 *)&object->pos_x_frac += object->vel_x;
}
