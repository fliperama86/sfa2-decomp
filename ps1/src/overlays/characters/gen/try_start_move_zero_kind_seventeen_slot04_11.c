/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 try_action_clearing_both_flags(Object *object);
void set_direction_index_from_buttons_group_a(Object *object);

/* functions of other units of this module */

int try_start_move_zero_kind_seventeen_slot04_11(Object *obj);

int try_start_move_zero_kind_seventeen_slot04_11(Object *obj) {
    if (obj->kind != 0x11) return 0;
    if (try_action_clearing_both_flags(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_a(obj);
        return 1;
    } else {
        return 0;
    }
}
