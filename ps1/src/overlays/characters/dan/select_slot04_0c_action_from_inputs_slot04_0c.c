/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 get_pressed_bit_8(Object *object);
int is_pressed_bits_in_table(Object *object);
int call_second_table_stage_unless_c0_pair(Object *object, u8 index, u8 arg);
int call_second_table_stage_unless_30_pair(Object *object, u8 index, u8 arg);
int call_second_table_stage_unless_0c_pair(Object *object, u8 index, u8 arg);
u8 try_action_clearing_both_flags(Object *object);
int is_not_state_2_five(Object *object);
int try_spend_meter_for_action(Object *object);
void set_direction_index_from_buttons_mask_a(Object *object);
void set_direction_index_from_buttons_mask_b(Object *object);
/* functions of other units of this module */
int try_enter_state_07_move_0_slot04_0c(Object *obj);
int try_enter_state_7_kind_1_unlinked_slot04_0c(Object *obj);
int try_enter_state_7_kind_3_slot04_0c(Object *obj);
void start_move_sequence_19_lock_other_slot04_0c(Object *obj);

int try_start_meter_move_type_c_slot04_0c(Object *obj);
int try_enter_state_7_via_0a24_state_3_0_slot04_0c(Object *obj);
int try_enter_state_7_via_0a24_state_3_3_slot04_0c(Object *obj);
void restore_facing_side_slot04_0c(Object *obj);
int try_start_meter_move_state_8_type_8_slot04_0c(Object *obj);
int try_enter_state_7_kind_7_or_9_slot04_0c(Object *obj);
int try_start_meter_move_type_b_with_input_slot04_0c(Object *obj);
int try_start_meter_move_type_4_link_check_slot04_0c(Object *obj);
int try_start_meter_action_state_8_type_6_slot04_0c(Object *obj);
int try_start_sequence_1a_lock_other_slot04_0c(Object *obj);
void start_move_sequence_1a_lock_other_slot04_0c(Object *obj);
int try_start_sequence_19_lock_other_slot04_0c(Object *obj);

void select_slot04_0c_action_from_inputs_slot04_0c(Object *obj) {
    if (call_slot_stage_handler_in_first_table(obj, 8, 0x18) && try_start_meter_move_type_c_slot04_0c(obj)) return;
    if (call_slot_stage_handler_in_first_table(obj, 9, 0x1f) && try_enter_state_7_via_0a24_state_3_0_slot04_0c(obj)) return;
    if (call_slot_stage_handler_in_first_table(obj, 10, 0x20) && try_enter_state_7_via_0a24_state_3_3_slot04_0c(obj)) return;
    if (get_pressed_bit_8(obj) && try_enter_state_7_kind_7_or_9_slot04_0c(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && try_start_meter_move_state_8_type_8_slot04_0c(obj)) return;
    if ((u8)call_second_table_stage_unless_c0_pair(obj, 0, 0x14) && try_start_meter_move_type_4_link_check_slot04_0c(obj)) return;
    if ((u8)call_second_table_stage_unless_30_pair(obj, 1, 0x15) && try_start_meter_action_state_8_type_6_slot04_0c(obj)) return;
    if ((u8)call_second_table_stage_unless_0c_pair(obj, 2, 0x17) && try_start_meter_move_type_b_with_input_slot04_0c(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 3, 4) && try_enter_state_07_move_0_slot04_0c(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 4, 0) && try_enter_state_7_kind_1_unlinked_slot04_0c(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 5, 3) && try_enter_state_7_kind_3_slot04_0c(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 6, 0xe) && try_start_sequence_1a_lock_other_slot04_0c(obj)) return;
    if ((u8)call_slot_stage_handler_in_second_table(obj, 7, 0xd)) try_start_sequence_19_lock_other_slot04_0c(obj);
}

int try_start_meter_move_type_c_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0xc;
        obj->active_flag_3 = 0;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int try_enter_state_7_via_0a24_state_3_0_slot04_0c(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        restore_facing_side_slot04_0c(obj);
    }
    return r;
}

int try_enter_state_7_via_0a24_state_3_3_slot04_0c(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_2 = 7;
        obj->state_1 = 0;
        obj->state_3 = 3;
        restore_facing_side_slot04_0c(obj);
    }
    return r;
}

void restore_facing_side_slot04_0c(Object *obj) {
    obj->facing_side = obj->saved_facing;
    obj->field_15a = 10;
    obj->active_flag_3 = 0;
}

int try_start_meter_move_state_8_type_8_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if ((u8)is_not_state_2_five(obj)) {
                    if ((u8)func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 8;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                if ((u8)try_action_unless_state_bytes_match(obj)) {
                    r = 1;
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 8;
                    obj->state_3 = 0;
                    obj->field_15a = 8;
                }
            }
        }
    }
    return r;
}

int try_enter_state_7_kind_7_or_9_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->active_state != 0) {
        if ((obj->vel_y & 0xffff0000) != 0) {
            if ((u8)try_action_unless_state_bytes_match(obj)) {
                r = 1;
                obj->state_0 = 1;
                obj->state_2 = 7;
                obj->state_1 = 0;
                obj->state_3 = 0;
                obj->field_15a = 9;
            }
        }
    } else if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 7;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int try_start_meter_move_type_b_with_input_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 0xb;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}

int try_start_meter_move_type_4_link_check_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags == 0) {
            if ((u8)func_80141788(obj)) {
                r = 1;
                obj->state_0 = 1;
                obj->state_1 = 0;
                obj->state_2 = 8;
                obj->state_3 = 0;
                obj->field_15a = 4;
                obj->active_flag_3 = 1;
                obj->facing_side = obj->saved_facing;
                set_direction_index_from_buttons_mask_a(obj);
            }
        }
    }
    return r;
}

int try_start_meter_action_state_8_type_6_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}

int try_start_sequence_1a_lock_other_slot04_0c(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_move_sequence_1a_lock_other_slot04_0c(obj);
    }
    return r;
}

void start_move_sequence_1a_lock_other_slot04_0c(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->facing_side = t;
    obj->field_15a = 2;
    obj->other_lock = 0x1b;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x17;
    set_sequence_by_side_and_mode(obj, 0x1a);
}

int try_start_sequence_19_lock_other_slot04_0c(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_move_sequence_19_lock_other_slot04_0c(obj);
    }
    return r;
}
