/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7a10_slot04_11[];
extern ObjectFn data_801c7a1c_slot04_11[];
extern ObjectFn data_801c7a24_slot04_11[];

void update_sequence_with_input_check(Object *object);

void func_801b5f64_slot04_11(Object *obj);
void apply_step_flags_offset_to_pos_x_slot04_11(Object *obj);
int apply_velocity_and_acceleration_2_slot04_11(Object *obj);
void func_801b604c_slot04_11(Object *obj);

void call_handler_by_input_dir_0_slot04_11(Object *obj) {
    data_801c7a10_slot04_11[obj->input_dir_0 >> 1](obj);
}

void call_handler_by_state_3_clear_field_2_slot04_11(Object *obj) {
    obj->field_157 = 0;
    data_801c7a1c_slot04_11[obj->state_3](obj);
}

void start_state_by_direction_or_switch_state_slot04_11(Object *obj) {
    obj->state_3++;
    if (obj->direction_index == 0) {
        func_801b5f64_slot04_11(obj);
    } else if (obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) && (func_8013f8c4(obj, -0x14, 0xe) & 0xff)) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b5f64_slot04_11(obj);
        if (obj->input_dir_second == 0 && obj->direction_index != 0) {
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
        }
        apply_step_flags_offset_to_pos_x_slot04_11(obj);
    }
}

void func_801b5c94_slot04_11(Object *obj) {
    update_sequence_with_input_check(obj);
    apply_step_flags_offset_to_pos_x_slot04_11(obj);
}

void call_handler_by_state_3_set_flag_2_slot04_11(Object *obj) {
    obj->field_157 = 1;
    data_801c7a24_slot04_11[obj->state_3](obj);
}

void advance_state_3_and_flag_on_direction_4_slot04_11(Object *obj) {
    obj->state_3++;
    func_801b5f64_slot04_11(obj);
    if (obj->input_dir_second != 0) {
        if (obj->direction_index == 4) {
            obj->field_278 = 1;
            apply_step_flags_offset_to_pos_x_slot04_11(obj);
        }
    } else if (obj->direction_index == 4) {
        obj->field_29c = 1;
        obj->sequence_hold_flag = 1;
    }
}

void func_801b5d8c_slot04_11(Object *obj) {
    update_sequence_with_input_check(obj);
    apply_step_flags_offset_to_pos_x_slot04_11(obj);
}

void update_airborne_land_or_change_sequence_slot04_11(Object *obj) {
    if (obj->active_flag != 0 && *(u8 *)&obj->step_flags != 0 && (obj->pressed_bits & 8)) {
        set_sequence_by_side_and_mode(obj, obj->variant_index != 0 ? 0x19 : 0x18);
    } else if (apply_velocity_and_acceleration_2_slot04_11(obj) < 0 && obj->pos_y >= obj->ground_y) {
        func_801b604c_slot04_11(obj);
        play_object_sound_by_kind(obj);
    } else {
        update_sequence(obj);
    }
}
