/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int is_pressed_bits_in_table(Object *object);
u8 get_pressed_bit_8(Object *object);
int call_slot_stage_handler_a(Object *object, u8 a, u8 b);
int call_second_table_stage_unless_0c_pair(Object *object, u8 a, u8 b);
int call_slot_stage_handler_a_2(Object *object, u8 a, u8 b);
int call_third_table_stage_unless_c0_pair(Object *object, u8 a, u8 b);
int call_third_table_stage_unless_30_pair(Object *object, u8 a, u8 b);
u8 try_action_clearing_both_flags(Object *object);
int try_spend_meter_for_action(Object *object);
void set_direction_index_from_buttons_mask_a(Object *object);
void set_direction_index_from_buttons_mask_b(Object *object);
void set_direction_index_from_buttons_group_a(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);
/* functions of other units of this module */
int try_start_meter_action_state_8_variant_a_slot04_12(Object *obj);
void clear_two_nine_byte_groups_unless_kind_4_slot04_12(Object *obj);
u8 enter_state_1_0_7_0_type_0_slot04_12(Object *obj);
u8 enter_state_1_0_7_0_type_1_slot04_12(Object *obj);
u8 enter_state_1_0_7_0_type_2_if_unlinked_slot04_12(Object *obj);
u8 try_start_action_state_7_variant_3_slot04_12(Object *obj);
u8 try_start_meter_action_state_8_variant_4_slot04_12(Object *obj);
u8 try_start_meter_action_state_8_variant_5_slot04_12(Object *obj);
u8 try_start_meter_action_state_8_variant_6_slot04_12(Object *obj);
u8 enter_state_1_0_7_0_type_7_if_remaining_slot04_12(Object *obj);
u8 try_start_meter_action_variant_8_slot04_12(Object *obj);
void start_action_variant_8_lock_other_slot04_12(Object *obj);
u8 try_start_meter_action_variant_9_slot04_12(Object *obj);
void select_slot04_12_action_from_inputs_slot04_12(Object *obj);

void select_slot04_12_action_from_inputs_slot04_12(Object *obj) {
    if ((u8)is_pressed_bits_in_table(obj) && (u8)try_start_meter_action_state_8_variant_a_slot04_12(obj)) return;
    if ((u8)call_second_table_stage_unless_0c_pair(obj, 0, 0x18) && try_start_meter_action_state_8_variant_6_slot04_12(obj)) return;
    if ((u8)call_third_table_stage_unless_30_pair(obj, 1, 6) && try_start_meter_action_state_8_variant_5_slot04_12(obj)) return;
    if ((u8)call_third_table_stage_unless_c0_pair(obj, 2, 5) && try_start_meter_action_state_8_variant_4_slot04_12(obj)) return;
    if ((u8)call_third_table_stage_unless_c0_pair(obj, 3, 2) && enter_state_1_0_7_0_type_2_if_unlinked_slot04_12(obj)) return;
    if ((u8)call_slot_stage_handler_a_2(obj, 4, 1) && try_start_action_state_7_variant_3_slot04_12(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 5, 0x13) && enter_state_1_0_7_0_type_1_slot04_12(obj)) return;
    if ((u8)call_slot_stage_handler_a_3(obj, 6, 1) && enter_state_1_0_7_0_type_0_slot04_12(obj)) return;
    if (get_pressed_bit_8(obj) && enter_state_1_0_7_0_type_7_if_remaining_slot04_12(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 7, 0xd) && try_start_meter_action_variant_8_slot04_12(obj)) return;
    if ((u8)call_slot_stage_handler_a(obj, 8, 0xe)) try_start_meter_action_variant_9_slot04_12(obj);
}

u8 enter_state_1_0_7_0_type_0_slot04_12(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 enter_state_1_0_7_0_type_1_slot04_12(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 1;
        obj->active_flag_3 = 1;
        obj->active_flag = 0;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 enter_state_1_0_7_0_type_2_if_unlinked_slot04_12(Object *obj) {
    u8 r = 0;

    if (obj->field_7e == 0) {
        if (obj->link_flags != 0) {
            return 0;
        }
    }
    if (try_action_clearing_both_flags(obj)) {
        r++;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

u8 try_start_action_state_7_variant_3_slot04_12(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        clear_two_nine_byte_groups_unless_kind_4_slot04_12(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 try_start_meter_action_state_8_variant_4_slot04_12(Object *obj) {
    int r = 0;

    if ((s16)obj->meter < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}

u8 try_start_meter_action_state_8_variant_5_slot04_12(Object *obj) {
    int r = 0;

    if ((s16)obj->meter < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        clear_two_nine_byte_groups_unless_kind_4_slot04_12(obj);
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}

u8 try_start_meter_action_state_8_variant_6_slot04_12(Object *obj) {
    int r = 0;

    if ((s16)obj->meter < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
    }
    return r;
}

u8 enter_state_1_0_7_0_type_7_if_remaining_slot04_12(Object *obj) {
    u8 r = 0;

    if (obj->field_7e == 0) {
        if (obj->remaining_count == 0) {
            return 0;
        }
    }
    if (try_action_clearing_both_flags(obj)) {
        r++;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 7;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

u8 try_start_meter_action_variant_8_slot04_12(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        start_action_variant_8_lock_other_slot04_12(obj);
    }
    return r;
}
