/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 get_pressed_bit_8(Object *object);
int is_pressed_bits_in_table(Object *object);
int func_8013cb70(Object *object, u8 index, u8 arg);
int func_8013cbdc(Object *object, u8 index, u8 arg);
int func_8013cc48(Object *object, u8 index, u8 arg);
int call_slot_stage_handler_a(Object *object, u8 index, u8 arg);
u8 try_action_clearing_both_flags(Object *object);
int is_not_state_2_five(Object *object);
int try_spend_meter_for_action(Object *object);
void set_direction_index_from_buttons_mask_a(Object *object);
void set_direction_index_from_buttons_mask_b(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);

/* functions of other units of this module */
int try_start_state_seven_move_nine_slot04_0d(Object *obj);
int try_start_meter_move_eight_slot04_0d(Object *obj);

int try_start_meter_move_type_1_slot04_0d(Object *obj);
int try_start_meter_move_six_slot04_0d(Object *obj);
int try_start_meter_move_seven_slot04_0d(Object *obj);
int try_start_state_seven_move_four_slot04_0d(Object *obj);
int try_start_move_state_7_type_0_slot04_0d(Object *obj);
int try_start_state_seven_move_five_slot04_0d(Object *obj);
int try_spend_meter_and_start_action_slot04_0d(Object *obj);
int try_spend_meter_and_start_action_2_slot04_0d(Object *obj);
void enter_state_2_seven_variant_two_sequence_slot04_0d(Object *obj);
void enter_state_2_seven_variant_three_sequence_slot04_0d(Object *obj);

void select_slot04_0d_action_from_inputs_slot04_0d(Object *obj) {
    if (get_pressed_bit_8(obj) && (u8)try_start_state_seven_move_nine_slot04_0d(obj)) return;
    if ((u8)is_pressed_bits_in_table(obj) && (u8)try_start_meter_move_type_1_slot04_0d(obj)) return;
    if ((u8)func_8013cb70(obj, 0, 0x18) && (u8)try_start_meter_move_six_slot04_0d(obj)) return;
    if ((u8)func_8013cbdc(obj, 1, 0x15) && (u8)try_start_meter_move_seven_slot04_0d(obj)) return;
    if ((u8)func_8013cc48(obj, 2, 0x17) && (u8)try_start_meter_move_eight_slot04_0d(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 3, 4) && (u8)try_start_state_seven_move_four_slot04_0d(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 4, 0) && (u8)try_start_move_state_7_type_0_slot04_0d(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 5, 3) && (u8)try_start_state_seven_move_five_slot04_0d(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 6, 0xd) && (u8)try_spend_meter_and_start_action_slot04_0d(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 7, 0xe)) try_spend_meter_and_start_action_2_slot04_0d(obj);
}

int try_start_move_state_7_type_0_slot04_0d(Object *obj) {
    if (obj->linked_object_3 != 0) return 0;
    if (!try_action_clearing_both_flags(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

int try_start_meter_move_type_1_slot04_0d(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state != 0) {
        if (obj->field_7e != 0) return 0;
        if (!(u8)try_action_unless_state_bytes_match(obj)) return 0;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 1;
    } else {
        if (!(u8)is_not_state_2_five(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 1;
        obj->facing_side = obj->saved_facing;
    }
    return 1;
}

int try_spend_meter_and_start_action_slot04_0d(Object *obj) {
    if ((u8)try_spend_meter_for_action(obj)) {
        enter_state_2_seven_variant_two_sequence_slot04_0d(obj);
        return 1;
    }
    return 0;
}

void enter_state_2_seven_variant_two_sequence_slot04_0d(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->facing_side = t;
    obj->field_15a = 2;
    obj->other_lock = 0x1c;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x18;
    set_sequence_by_side_and_mode(obj, 0x2e);
}

int try_spend_meter_and_start_action_2_slot04_0d(Object *obj) {
    if ((u8)try_spend_meter_for_action(obj)) {
        enter_state_2_seven_variant_three_sequence_slot04_0d(obj);
        return 1;
    }
    return 0;
}

void enter_state_2_seven_variant_three_sequence_slot04_0d(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->facing_side = t;
    obj->field_15a = 3;
    obj->other_lock = 0x20;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x1c;
    set_sequence_by_side_and_mode(obj, 0x2e);
}

int try_start_state_seven_move_four_slot04_0d(Object *obj) {
    u8 one;
    int r;

    if (try_action_clearing_both_flags(obj)) {
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->active_flag_3 = one;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

int try_start_state_seven_move_five_slot04_0d(Object *obj) {
    u8 one;
    int r;

    if (try_action_clearing_both_flags(obj)) {
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = one;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

int try_start_meter_move_six_slot04_0d(Object *obj) {
    if ((s16)obj->meter < 0x30) goto z;
    if ((u8)func_80141788(obj)) goto b;
z:
    return 0;
b:
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
    return 1;
}

int try_start_meter_move_seven_slot04_0d(Object *obj) {
    if ((s16)obj->meter < 0x30) goto z;
    if ((u8)func_80141788(obj)) goto b;
z:
    return 0;
b:
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_mask_b(obj);
    return 1;
}
