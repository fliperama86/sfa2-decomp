/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_object_states_0_to_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 try_enter_meter_action_state_8_slot04_02(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if (is_not_state_2_five(obj) & 0xff) {
                    r = func_80141788(obj);
                    if (r) {
                        set_object_states_0_to_3_slot04_02(obj, 1, 0, 8, 0);
                        obj->field_15a = 0xf;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                r = try_action_unless_state_bytes_match(obj);
                if (r) {
                    set_object_states_0_to_3_slot04_02(obj, 1, 0, 8, 0);
                    obj->field_15a = 0xf;
                }
            }
        }
    }
    return r;
}

u8 try_enter_state_7_with_action_code_e_slot04_02(Object *obj) {
    u8 r = try_action_clearing_both_flags(obj);

    if (r) {
        set_object_states_0_to_3_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0xe;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}
