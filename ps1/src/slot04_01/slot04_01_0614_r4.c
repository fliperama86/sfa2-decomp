/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_07_move_2_or_3_slot04_01(Object *obj) {
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
