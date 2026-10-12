/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_enter_state_07_move_1_if_linked_slot04_01(Object *obj) {
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
        obj->field_15a = 1;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
    }
    return r;
}
