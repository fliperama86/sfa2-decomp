/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 try_action_clearing_both_flags(Object *object);
int is_not_state_2_five(Object *object);
/* function of another unit of this module */
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void set_input_dir_second_from_buttons_slot04_14(Object *obj);
u8 try_start_effect_by_state(Object *object);
int try_spend_meter_for_action(Object *object);
void func_80142718(Object *object);

u8 func_801b0ce0_slot04_14(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if (is_not_state_2_five(obj) & 0xff) {
                    r = func_80141788(obj);
                    if (r) {
                        set_object_states_0_to_3_slot04_14(obj, 1, 0, 8, 0);
                        obj->field_15a = 0xf;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                r = func_801418bc(obj);
                if (r) {
                    set_object_states_0_to_3_slot04_14(obj, 1, 0, 8, 0);
                    obj->field_15a = 0xf;
                }
            }
        }
    }
    return r;
}

u8 func_801b0ddc_slot04_14(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0xe;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

u8 func_801b0e48_slot04_14(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0xd;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

u8 func_801b0eb4_slot04_14(Object *obj) {
    u8 r;

    if (obj->field_7e == 0 && obj->remaining_count == 0) return;
    r = try_action_clearing_both_flags(obj);
    if (r) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0xc;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

u8 func_801b0f3c_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x90) {
        r = try_start_effect_by_state(obj);
        if (r) {
            set_object_states_0_to_3_slot04_14(obj, 1, 0, 8, 0);
            obj->field_15a = 0xb;
            obj->direction_index = 4;
            obj->field_255 = 4;
            obj->facing_side = obj->saved_facing;
        }
    }
    return r;
}

u8 try_start_action_08_opposite_variant_slot04_14(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);
    u8 t;

    if (r) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        t = obj->saved_facing;
        obj->field_15a = 8;
        obj->facing_side = t;
        obj->variant_index = t ^ 1;
        set_input_dir_second_from_buttons_slot04_14(obj);
    }
    return r;
}

u8 try_enter_move_state_with_same_variant_slot04_14(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 8;
        obj->variant_index = obj->facing_side = obj->saved_facing;
        set_input_dir_second_from_buttons_slot04_14(obj);
    }
    return r;
}

void set_input_dir_second_from_buttons_slot04_14(Object *obj) {
    u16 t;

    t = obj->pressed_bits | obj->released_bits;
    obj->input_dir_second = 0;
    obj->direction_index = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->input_dir_second = obj->input_dir_second + 2;
    }
}

u8 try_start_meter_move_7_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        r = func_80141788(obj);
        if (r) {
            set_object_states_0_to_3_slot04_14(obj, 1, 0, 8, 0);
            obj->field_15a = 7;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            func_80142718(obj);
        }
    }
    return r;
}

u8 func_801b1180_slot04_14(Object *obj) {
    int r = 1;

    if (obj->mode_flag_2 == 0) {
        r = try_spend_meter_for_action(obj);
        if (!(u8)r) return 0;
    }
    set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->other_lock = 0x1e;
    obj->facing_side = obj->saved_facing;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->other->field_6b = 0x1a;
    set_sequence_by_side_and_mode(obj, 0x1a);
    return r;
}

void func_801b1238_slot04_14(Object *obj) {
    set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->other_lock = 0x1e;
    obj->facing_side = obj->saved_facing;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->other->field_6b = 0x1a;
    set_sequence_by_side_and_mode(obj, 0x1a);
}

u8 func_801b12b0_slot04_14(Object *obj) {
    int r = 1;

    if (obj->mode_flag_2 == 0) {
        r = try_spend_meter_for_action(obj);
        if (!(u8)r) return 0;
    }
    set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 5);
    obj->active_flag_3 = 1;
    obj->other_lock = 0x19;
    obj->facing_side = obj->saved_facing;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->field_15a = 0;
    obj->other->field_6b = 0x15;
    set_sequence_by_side_and_mode(obj, 0x31);
    return r;
}

void func_801b1368_slot04_14(Object *obj) {
    set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 5);
    obj->active_flag_3 = 1;
    obj->other_lock = 0x19;
    obj->facing_side = obj->saved_facing;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->field_15a = 0;
    obj->other->field_6b = 0x15;
    set_sequence_by_side_and_mode(obj, 0x31);
}

u8 func_801b13e0_slot04_14(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}
