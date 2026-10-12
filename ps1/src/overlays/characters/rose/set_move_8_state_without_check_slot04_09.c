/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_move_8_state_without_check_slot04_09(Object *obj) {
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->field_15a = 8;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x10;
    obj->direction_index = 0;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
}

int try_start_move_0_unless_linked_slot04_09(Object *obj) {
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
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

int try_start_move_1_slot04_09(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 1;
        obj->active_flag_3 = 1;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}
