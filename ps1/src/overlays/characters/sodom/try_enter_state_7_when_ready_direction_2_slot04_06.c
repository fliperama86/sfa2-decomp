/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_enter_state_7_when_ready_direction_2_slot04_06(Object *obj) {
    if (!(u8)is_state_01010101_and_ready(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 6;
    obj->active_flag_3 = 1;
    obj->direction_index = 2;
    return 1;
}

int try_enter_state_7_kind_7_slot04_06(Object *obj) {
    if (obj->field_7e != 0 || obj->remaining_count != 0) {
        if (try_action_clearing_both_flags(obj) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->field_15a = 7;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            return 1;
        }
    }
    return 0;
}
