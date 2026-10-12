/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b110c_slot04_09(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        obj->field_15a = 3;
        obj->active_flag_3 = 1;
        obj->state_0 = 1;
        obj->state_2 = 7;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_6b = 0;
        obj->other->field_6b = 0xc;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}

int func_801b11a4_slot04_09(Object *obj) {
    int r = 0;

    if ((u8)try_spend_meter_for_action(obj)) {
        r = 1;
        obj->field_15a = 8;
        obj->active_flag_3 = 1;
        obj->state_0 = 1;
        obj->state_2 = 7;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_6b = 0;
        obj->other->field_6b = 0x10;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

void set_move_3_state_without_check_slot04_09(Object *obj) {
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->field_15a = 3;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0xc;
    obj->direction_index = 0;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
}
