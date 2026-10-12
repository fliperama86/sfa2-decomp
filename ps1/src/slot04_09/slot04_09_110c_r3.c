/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_start_move_2_slot04_09(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 2;
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

int try_enter_action_move_without_meter_cost_slot04_09(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0xa;
        obj->active_flag_3 = 1;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}
