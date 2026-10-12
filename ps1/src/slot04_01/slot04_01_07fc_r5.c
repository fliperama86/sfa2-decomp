/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0e04_slot04_01(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0xa;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int try_enter_state_07_move_7_with_count_slot04_01(Object *obj) {
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
        obj->facing_side = obj->saved_facing;
    }
    return r;
}
