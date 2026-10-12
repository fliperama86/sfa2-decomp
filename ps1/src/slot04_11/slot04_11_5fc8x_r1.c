/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enable_active_flag_3_and_select_sequence_3_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_11(Object *obj);
void call_state_3_handler_wrapper_3_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);
void enable_active_flag_3_and_select_sequence_3_slot04_11(Object *obj);
void shift_pos_x_by_step_flags_low_bits_slot04_11(Object *obj);
int apply_velocity_and_acceleration_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_sequence_0_3_slot04_11(Object *obj);
void call_reset_object_to_state_1_0_0_2_sequence_1_3_slot04_11(Object *obj);
void call_state_3_handler_wrapper_3_slot04_11(Object *obj);
void start_direction_sequence_slot04_11(Object *obj);

int apply_velocity_and_acceleration_2_slot04_11(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_x += obj->accel_x;
    return obj->vel_y += obj->accel_y;
}
