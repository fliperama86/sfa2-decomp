/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void set_direction_index_from_buttons_mask_a(Object *object);
void set_direction_index_from_buttons_mask_b(Object *object);
void set_direction_index_from_buttons_group_b(Object *object);

u8 try_enter_state_8_action_b_with_meter_slot04_16(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30 && func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 0xb;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_a(obj);
        return 1;
    }
    return r;
}

u8 try_start_meter_action_type_a_both_flags_slot04_16(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30 && func_80141788(obj) && try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 0xa;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
        return 1;
    }
    return r;
}

u8 try_start_move_7_with_dir_slot04_16(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

u8 try_start_move_8_with_dir_slot04_16(Object *obj) {
    u8 r = 0;

    if (*(u16 *)&obj->state_0 == 1 && obj->state_2 == 5) {
        if (obj->active_flag == 0) {
            if (try_action_clearing_both_flags(obj)) {
                r = 1;
                obj->field_15a = 8;
                obj->state_0 = 1;
                obj->state_1 = 0;
                obj->state_2 = 7;
                obj->state_3 = 0;
                obj->active_flag_3 = 1;
                obj->facing_side = obj->saved_facing;
                set_direction_index_from_buttons_group_b(obj);
                return 1;
            }
        }
    } else if (try_action_clearing_both_flags(obj)) {
        r++;
        obj->field_15a = 8;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 try_start_move_6_with_dir_slot04_16(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

u8 try_start_move_4_with_dir_slot04_16(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 4;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

u8 try_start_move_5_slot04_16(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) {
        return 0;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 5;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    return 1;
}
