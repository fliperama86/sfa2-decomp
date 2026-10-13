/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);

extern ObjectFnInt data_801c5a30_slot04_0f[];
extern ObjectFn data_801c5a60_slot04_0f[];
extern ObjectFn data_801c5a90_slot04_0f[];

void clear_direction_and_bump_slot_stage_slot04_0f(Object *obj);
void start_special_type_9_without_check_slot04_0f(Object *obj);
int return_true_always_slot04_0f(Object *obj);

void start_special_type_9_without_check_slot04_0f(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->field_15a = 9;
    obj->other_lock = 0x1d;
    obj->facing_side = t;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x19;
    set_sequence_by_side_and_mode(obj, 0x48);
}

int try_start_move_variant_same_facing_slot04_0f(Object *o) {
    Slot *p = &o->slots[16];
    int ok;

    if (o->active_state != 0) {
        if (p->timer != 0) return 0;
        ok = (u8)try_action_unless_state_bytes_match(o);
    } else {
        ok = try_action_clearing_both_flags(o);
    }
    if (ok) {
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 7;
        o->state_3 = 0;
        o->field_15a = 0;
        o->variant_index = o->facing_side = o->saved_facing;
        p[1].stage = 0;
        clear_direction_and_bump_slot_stage_slot04_0f(o);
        return 1;
    }
    return 0;
}

int try_start_move_variant_flipped_facing_slot04_0f(Object *o) {
    Slot *p = &o->slots[16];
    int ok;
    u8 t;

    if (o->active_state != 0) {
        if (p->timer != 0) return 0;
        ok = (u8)try_action_unless_state_bytes_match(o);
    } else {
        ok = try_action_clearing_both_flags(o);
    }
    if (ok) {
        o->state_0 = 1;
        o->state_2 = 7;
        o->state_1 = 0;
        o->state_3 = 0;
        o->field_15a = 0;
        t = o->saved_facing;
        o->facing_side = t;
        o->variant_index = t ^ 1;
        p[1].stage = 4;
        clear_direction_and_bump_slot_stage_slot04_0f(o);
        return 1;
    }
    return 0;
}

void clear_direction_and_bump_slot_stage_slot04_0f(Object *obj) {
    u16 t;

    t = obj->pressed_bits | obj->released_bits;
    obj->direction_index = 0;
    obj->active_flag_3 = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->slots[17].stage = obj->slots[17].stage + 2;
    }
}

int try_start_move_1_with_dir_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (!try_action_clearing_both_flags(obj)) return 0;
    one = 1;
    obj->state_0 = one;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = one;
    obj->input_dir_second = 0;
    obj->active_flag_3 = one;
    t = obj->saved_facing;
    obj->facing_side = t;
    obj->variant_index = t;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

int try_start_move_2_with_dir_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (!try_action_clearing_both_flags(obj)) return 0;
    one = 1;
    obj->state_0 = one;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 2;
    obj->input_dir_second = 0;
    obj->active_flag_3 = one;
    t = obj->saved_facing;
    obj->facing_side = t;
    obj->variant_index = t;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

int try_start_move_3_with_dir_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (!try_action_clearing_both_flags(obj)) return 0;
    one = 1;
    obj->state_0 = one;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 3;
    obj->input_dir_second = 0;
    obj->active_flag_3 = one;
    t = obj->saved_facing;
    obj->facing_side = t;
    obj->variant_index = t;
    set_direction_index_from_buttons_group_b(obj);
    return 1;
}

void func_801b1a90_slot04_0f(Object *obj) {
    script_condition_result = data_801c5a30_slot04_0f[obj->field_15a](obj);
}

int set_slot_17_stage_from_inputs_slot04_0f(Object *obj) {
    u8 t;

    t = obj->saved_facing;
    obj->slots[17].stage = 0;
    obj->facing_side = t;
    obj->variant_index = t;
    if (obj->direction_flag != 0) {
        obj->slots[17].stage = 4;
    }
    if (obj->input_dir_second != 0) {
        obj->slots[17].stage = obj->slots[17].stage + 2;
    }
    return return_true_always_slot04_0f(obj);
}

int has_meter_and_no_link_flags_slot04_0f(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->link_flags != 0) return 0;
    return return_true_always_slot04_0f(obj);
}

int has_no_link_flags_slot04_0f(Object *obj) {
    if (obj->link_flags != 0) return 0;
    return return_true_always_slot04_0f(obj);
}

int has_meter_at_least_threshold_slot04_0f(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    return return_true_always_slot04_0f(obj);
}

int is_remaining_count_nonzero_slot04_0f(Object *obj) {
    if (obj->remaining_count == 0) return 0;
    return return_true_always_slot04_0f(obj);
}

int is_state_2222_and_healthy_slot04_0f(Object *obj) {
    if (*(s32 *)&obj->state_0 != 0x2020202) return 0;
    if (obj->active_state != 0) return 0;
    if (obj->field_163 != 0) return 0;
    if ((s16)obj->health < 0) return 0;
    return return_true_always_slot04_0f(obj);
}

int return_true_always_slot04_0f(Object *obj) {
    return 1;
}

int return_zero_slot04_0f(Object *obj) {
    return 0;
}

void func_801b1c84_slot04_0f(Object *obj) {
    data_801c5a60_slot04_0f[obj->field_15a](obj);
}

void func_801b1cc4_slot04_0f(Object *obj) {
    data_801c5a90_slot04_0f[obj->field_15a](obj);
}
