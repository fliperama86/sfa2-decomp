/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_start_meter_move_state_8_type_5_slot04_00(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_mask_b(obj);
    }
    return r;
}

int try_enter_state_07_move_2_or_3_slot04_00(Object *obj) {
    int r = 0;

    if (obj->active_state != 0) {
        if (!(u8)try_action_unless_state_bytes_match(obj)) return 0;
        r = 1;
        obj->field_15a = 3;
    } else {
        if (!try_action_clearing_both_flags(obj)) return 0;
        r = 1;
        obj->field_15a = 2;
        obj->facing_side = obj->saved_facing;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->active_flag_3 = 1;
    set_direction_index_from_buttons_group_b(obj);
    return r;
}
