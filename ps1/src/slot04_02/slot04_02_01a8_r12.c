/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 try_enter_state_7_with_action_code_zero_slot04_02(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

u8 try_enter_move_state_grounded_or_airborne_slot04_02(Object *obj) {
    u8 r;
    int t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (obj->active_state != 0) {
        r = try_action_unless_state_bytes_match(obj);
        if (!r) return;
        t = (u16)obj->ground_y;
        t -= 0x30;
        if (obj->pos_y >= (s16)t) return 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        set_direction_index_from_buttons_group_a(obj);
    } else {
        r = try_action_clearing_both_flags(obj);
        if (r) {
            obj->field_15a = 1;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
            set_direction_index_from_buttons_group_a(obj);
        }
    }
    return r;
}
