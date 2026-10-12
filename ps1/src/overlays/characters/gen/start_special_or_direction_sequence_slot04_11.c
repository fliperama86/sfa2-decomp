/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void func_801b1448(Object *obj);
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
void start_special_or_direction_sequence_slot04_11(Object *obj);

void start_special_or_direction_sequence_slot04_11(Object *obj) {
    obj->input_dir_second = 2;
    if (obj->kind != 0x13 || *(s16 *)&obj->meter < 0x30 || obj->active_state != 0 || obj->ground_y - 0x18 < obj->pos_y) {
        start_direction_sequence_slot04_11(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 7;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_801b1448(obj);
    }
}
