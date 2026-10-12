/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
extern ObjectFnInt data_801c61d8_slot04_02[];

u8 try_start_meter_action_state_8_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags != 0) {
            return 0;
        }
        if (obj->active_state != 0) {
            r = try_action_unless_state_bytes_match(obj);
            if (r != 0) {
                obj->field_4b = obj->field_25c;
                set_object_states_0_to_3_slot04_02(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->active_flag_3 = 1;
                set_direction_index_from_buttons_mask_a(obj);
            }
        }
    }
    return r;
}

u8 try_enter_state_7_with_action_code_ten_slot04_02(Object *obj) {
    u8 r;

    if (obj->field_7e != 0 || obj->link_flags == 0) {
        r = try_action_clearing_both_flags(obj);
        if (r != 0) {
            obj->field_15a = 10;
            obj->facing_side = obj->saved_facing;
            set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
            obj->active_flag_3 = 1;
            set_direction_index_from_buttons_group_a(obj);
        }
        return r;
    }
    return 0;
}

void func_801b1730_slot04_02(Object *obj) {
    data_801ad398 = data_801c61d8_slot04_02[obj->field_15a](obj);
}

int return_true_always_slot04_02(Object *obj) {
    return 1;
}

int is_link_flags_zero_slot04_02(Object *obj) {
    return obj->link_flags == 0;
}
