/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 try_enter_move_state_with_input_direction_slot04_02(Object *obj) {
    u8 r;

    if (obj->active_state != 0) {
        r = try_action_unless_state_bytes_match(obj);
        if (!r) return;
        obj->field_15a = 3;
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        obj->active_flag_3 = 1;
        set_direction_index_from_buttons_group_b(obj);
    } else {
        r = try_action_clearing_both_flags(obj);
        if (!r) return;
        obj->field_15a = 2;
        obj->facing_side = obj->saved_facing;
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        obj->active_flag_3 = 1;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

u8 try_start_meter_move_5_unlinked_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags != 0) return 0;
        r = func_80141788(obj);
        if (r) {
            set_object_states_0_to_3_slot04_02(obj, 1, 0, 8, 0);
            obj->field_15a = 5;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            set_direction_index_from_buttons_mask_a(obj);
        }
    }
    return r;
}
