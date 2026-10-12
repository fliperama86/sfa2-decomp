/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1280_slot04_09(Object *obj) {
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

int func_801b12c4_slot04_09(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->link_flags != 0) {
            return 0;
        }
    }
    if (func_801417cc(obj)) {
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
        func_80142b3c(obj);
    }
    return r;
}

int func_801b1368_slot04_09(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
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
        func_80142b3c(obj);
    }
    return r;
}
