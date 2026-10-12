/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 enter_state_1_0_7_0_type_0_slot04_04(Object *obj) {
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

u8 enter_state_1_0_7_0_type_1_slot04_04(Object *obj) {
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

u8 enter_state_1_0_7_0_type_2_if_unlinked_slot04_04(Object *obj) {
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
