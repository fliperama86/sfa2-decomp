/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_start_meter_move_type_9_slot04_09(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if ((u8)is_not_state_2_five(obj)) {
                    if ((u8)func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 9;
                        obj->active_flag_3 = 1;
                    }
                }
            } else if (obj->field_7e == 0) {
                if ((u8)try_action_unless_state_bytes_match(obj)) {
                    r = 1;
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 8;
                    obj->state_3 = 0;
                    obj->field_15a = 9;
                }
            }
        }
    }
    return r;
}

int try_start_move_7_with_remaining_count_slot04_09(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->remaining_count == 0) {
            return 0;
        }
    }
    if (try_action_clearing_both_flags(obj) != 0) {
        r++;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 7;
        obj->active_flag_3 = 0;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}
