/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_7_kind_9_slot04_07(Object *obj) {
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
        obj->active_flag_3 = one;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}
