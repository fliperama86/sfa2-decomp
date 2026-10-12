/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void set_direction_index_from_buttons_mask_a(Object *object);
void set_object_states_0_to_3_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);

extern ObjectFnInt data_801c61b0_slot04_14[];
extern ObjectFn data_801c61f0_slot04_14[];
extern ObjectFn data_801c6230_slot04_14[];
extern ObjectFn data_801c6270_slot04_14[];

u8 try_start_meter_action_state_8_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags != 0) {
            return 0;
        }
        if (obj->active_state != 0) {
            r = try_action_unless_state_bytes_match(obj);
            if (r != 0) {
                obj->field_4b = obj->field_25c;
                set_object_states_0_to_3_slot04_14(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->active_flag_3 = 1;
                set_direction_index_from_buttons_mask_a(obj);
            }
        }
    }
    return r;
}

u8 try_enter_state_7_with_action_code_ten_slot04_14(Object *obj) {
    u8 r;

    if (obj->field_7e != 0 || obj->link_flags == 0) {
        r = try_action_clearing_both_flags(obj);
        if (r != 0) {
            obj->field_15a = 10;
            obj->facing_side = obj->saved_facing;
            set_object_states_0_to_3_slot04_14(obj, 1, 0, 7, 0);
            obj->active_flag_3 = 1;
            set_direction_index_from_buttons_group_a(obj);
        }
        return r;
    }
    return 0;
}

void func_801b17fc_slot04_14(Object *obj) {
    data_801ad398 = data_801c61b0_slot04_14[obj->field_15a](obj);
}

int return_true_always_slot04_14(Object *obj) {
    return 1;
}

int is_link_flags_zero_slot04_14(Object *obj) {
    return obj->link_flags == 0;
}

int is_active_state_set_slot04_14(Object *obj) {
    return obj->active_state != 0;
}

int is_airborne_without_link_slot04_14(Object *obj) {
    int r = 0;
    int t;

    if (obj->link_flags == 0) {
        if (obj->active_state != 0) {
            t = (u16)obj->ground_y;
            t -= 0x30;
            r = obj->pos_y < (s16)t;
        }
    }
    return r;
}

int has_meter_30_and_no_link_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        r = obj->link_flags == 0;
    }
    return r;
}

int is_meter_at_least_threshold_slot04_14(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int has_meter_30_active_and_no_link_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->active_state != 0) {
            r = obj->link_flags == 0;
        }
    }
    return r;
}

int is_meter_full_slot04_14(Object *obj) {
    return (s16)obj->meter >= 0x90;
}

int has_remaining_count_slot04_14(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b1940_slot04_14(Object *obj) {
    data_801c61f0_slot04_14[obj->field_15a](obj);
}

void func_801b1980_slot04_14(Object *obj) {
    data_801c6230_slot04_14[obj->field_15a](obj);
}

void call_table_function_by_state_3_4_slot04_14(Object *obj) {
    data_801c6270_slot04_14[obj->state_3](obj);
}
