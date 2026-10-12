/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b14fc_slot04_09(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        obj->substate_index = 0;
        obj->field_12d = 0;
        obj->step_count_2 = 0;
        obj->step_limit = 0;
        obj->facing_side = obj->saved_facing;
        func_80142718(obj);
    }
    return r;
}

int func_801b1598_slot04_09(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags != 0) {
            return 0;
        }
        if ((u8)func_80141788(obj)) {
            r = 1;
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 8;
            obj->state_3 = 0;
            obj->field_15a = 5;
            obj->active_flag_3 = 1;
            obj->substate_index = 0;
            obj->field_12d = 0;
            obj->step_count_2 = 0;
            obj->step_limit = 0;
            obj->facing_side = obj->saved_facing;
            func_80142718(obj);
        }
    }
    return r;
}
