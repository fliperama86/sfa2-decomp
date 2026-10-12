/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007e868_slot16[];
void call_handler_by_state_3_clear_field_slot16(Object *obj);
void call_handler_by_state_3_set_flag_slot16(Object *obj);
void set_active_flag_3_and_call_direction_sequence_slot16(Object *obj);
void func_801b606c(Object *obj);
void overlay_left_kind(Object *obj);

void update_by_attack_param_or_input_dir_slot16(Object *obj) {
    if (obj->attack_param_d != 0) {
        obj->kind = 0x13;
        overlay_left_kind(obj);
        select_box_tables(obj);
        build_metrics(obj);
        func_801b606c(obj);
    } else if (obj->input_dir_0 != 0) {
        call_handler_by_state_3_set_flag_slot16(obj);
    } else {
        call_handler_by_state_3_clear_field_slot16(obj);
    }
}

void call_handler_by_state_3_clear_field_slot16(Object *obj) {
    obj->field_157 = 0;
    data_8007e868_slot16[obj->state_3](obj);
}

void restore_facing_enter_state_or_call_start_helper_slot16(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 0 || obj->hit_flag_a == 0 || (u8)try_force_other_state_if_box_in_range(obj, -0x17, 0x14) == 0) {
        set_active_flag_3_and_call_direction_sequence_slot16(obj);
    } else {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
}
