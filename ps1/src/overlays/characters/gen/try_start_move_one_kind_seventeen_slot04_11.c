/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 try_action_clearing_both_flags(Object *object);
int try_spend_meter_for_action(Object *object);
int is_not_state_2_five(Object *object);
void set_direction_index_from_buttons_mask_a(Object *object);
void set_direction_index_from_buttons_mask_b(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);
void spawn_offset_effect_and_play_sounds(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);

extern ObjectFnInt data_801c732c_slot04_11[];
extern ObjectFn data_801c7354_slot04_11[];

void start_move_10_lock_other_slot04_11(Object *obj);
void start_move_11_lock_other_slot04_11(Object *obj);
int is_kind_seventeen_slot04_11(Object *obj);
int is_kind_nineteen_slot04_11(Object *obj);
void copy_kind_table_to_scratch_by_side_slot04_11(Object *obj);

int try_start_move_one_kind_seventeen_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if (try_action_clearing_both_flags(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 1;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
        return 1;
    } else {
        return 0;
    }
}

int try_start_move_two_kind_nineteen_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if (try_action_clearing_both_flags(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
        return 1;
    } else {
        return 0;
    }
}

int try_start_move_three_kind_nineteen_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if (try_action_clearing_both_flags(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
        return 1;
    } else {
        return 0;
    }
}

int start_move_4_if_kind_11_meter_ok_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if ((u8)func_80141788(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
        return 1;
    } else {
        return 0;
    }
}

int start_move_5_if_kind_11_meter_ok_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if ((u8)func_80141788(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
        return 1;
    } else {
        return 0;
    }
}

int start_move_6_if_kind_13_meter_ok_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if ((u8)func_80141788(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
        return 1;
    } else {
        return 0;
    }
}

int start_move_7_if_kind_13_airborne_meter_ok_slot04_11(Object *obj) {
    if (obj->kind != 0x13) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state == 0) return 0;
    if (!(obj->pos_y < obj->ground_y - 0x18)) return 0;
    if ((u8)try_action_unless_state_bytes_match(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 7;
        obj->active_flag_3 = 1;
        obj->field_4b = obj->field_25c;
        set_direction_index_from_buttons_mask_b(obj);
        return 1;
    } else {
        return 0;
    }
}

int start_move_8_or_count_failure_slot04_11(Object *obj) {
    if (*(u16 *)&obj->state_0 != 1) goto c;
    if (obj->state_2 == 5) goto z;
c:
    if (try_action_clearing_both_flags(obj)) goto b;
z:
    obj->flags_28b++;
    return 0;
b:
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 8;
    obj->facing_side = obj->saved_facing;
    return 1;
}

int start_move_9_if_allowed_slot04_11(Object *obj) {
    if (obj->field_7e != 0) goto c;
    if (obj->remaining_count == 0) goto z;
c:
    if (try_action_clearing_both_flags(obj)) goto b;
z:
    return 0;
b:
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->facing_side = obj->saved_facing;
    return 1;
}

int try_start_move_ten_spending_meter_slot04_11(Object *obj) {
    if (try_spend_meter_for_action(obj)) {
        start_move_10_lock_other_slot04_11(obj);
        return 1;
    }
    return 0;
}

void start_move_10_lock_other_slot04_11(Object *obj) {
    int a = 0x10;
    int b = 0x14;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0xa;
    obj->active_flag_3 = 1;
    obj->direction_index = 4;
    obj->facing_side = obj->saved_facing;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x13;
        b = 0x17;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->other_lock = b;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x1e);
}

int try_start_move_eleven_spending_meter_slot04_11(Object *obj) {
    if (try_spend_meter_for_action(obj)) {
        start_move_11_lock_other_slot04_11(obj);
        return 1;
    }
    return 0;
}

void start_move_11_lock_other_slot04_11(Object *obj) {
    int a = 0x13;
    int b = 0x17;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0xb;
    obj->active_flag_3 = 1;
    obj->direction_index = 4;
    obj->facing_side = obj->saved_facing;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x18;
        b = 0x1c;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->other_lock = b;
    spawn_offset_effect_and_play_sounds(obj);
    set_sequence_by_side_and_mode(obj, 0x1f);
}

int start_move_12_if_meter_ok_slot04_11(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state == 0) {
        if (!is_not_state_2_five(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->state_0 = 1;
        obj->state_2 = 8;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_15a = 0xc;
        obj->facing_side = obj->saved_facing;
        return 1;
    }
    if (obj->field_7e != 0) return 0;
    if (!(u8)try_action_unless_state_bytes_match(obj)) return 0;
    obj->state_0 = 1;
    obj->state_2 = 8;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 0xc;
    return 1;
}

void sync_kind_then_call_move_handler_slot04_11(Object *obj) {
    u8 k;

    if (obj->field_15a < 8) {
        k = 0x11;
        if (obj->attack_param_d != 0) {
            k = 0x13;
        }
        if (obj->kind != k) {
            obj->kind = k;
            copy_kind_table_to_scratch_by_side_slot04_11(obj);
            select_box_tables(obj);
            build_metrics(obj);
        }
    }
    script_condition_result = data_801c732c_slot04_11[obj->field_15a](obj);
}

int is_kind_seventeen_slot04_11(Object *obj) {
    return obj->kind == 0x11;
}

int check_kind_equals_seventeen_slot04_11(Object *obj) {
    return is_kind_seventeen_slot04_11(obj);
}

int is_kind_nineteen_slot04_11(Object *obj) {
    return obj->kind == 0x13;
}

int check_kind_equals_seventeen_2_slot04_11(Object *obj) {
    return is_kind_nineteen_slot04_11(obj);
}

int func_801b138c_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    return (s16)obj->meter >= 0x30;
}

int func_801b13bc_slot04_11(Object *obj) {
    if (obj->kind == 0x11) {
        return (s16)obj->meter >= 0x30;
    }
    return 0;
}

int func_801b13e4_slot04_11(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->meter >= 0x30;
    }
    return 0;
}

int is_kind_nineteen_with_small_meter_slot04_11(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->meter >= 6;
    }
    return 0;
}

int return_true_always_slot04_11(Object *obj) {
    return 1;
}

int has_remaining_count_slot04_11(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b1448_slot04_11(Object *obj) {
    data_801c7354_slot04_11[obj->field_15a](obj);
}
