/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_enter_state_07_move_1_if_linked_slot04_00(Object *obj) {
    int r = 0;

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
        obj->field_15a = 1;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

int try_enter_state_07_move_9_slot04_00(Object *obj) {
    int r = 0;
    u8 one;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 9;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

/* Declared int although it returns nothing itself: the caller tests the result register as the last call left it. */
int try_start_meter_move_state_8_type_4_slot04_00(Object *obj) {
    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags == 0) {
            if ((u8)func_80141788(obj)) {
                obj->state_0 = 1;
                obj->state_1 = 0;
                obj->state_2 = 8;
                obj->state_3 = 0;
                obj->field_15a = 4;
                obj->active_flag_3 = 1;
                obj->facing_side = obj->saved_facing;
                set_direction_index_from_buttons_mask_a(obj);
            }
        }
    }
}
