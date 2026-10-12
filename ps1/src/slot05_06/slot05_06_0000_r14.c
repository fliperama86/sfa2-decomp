/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_8_kind_9_slot05_06(Object *obj);

int try_start_meter_move_15a_9_slot05_06(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state != 0) {
        return (u8)try_enter_state_8_kind_9_slot05_06(obj);
    }
    if (!(u8)is_not_state_2_five(obj)) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->state_0 = 1;
    obj->state_2 = 8;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->facing_side = obj->saved_facing;
    return 1;
}

int try_enter_state_8_kind_9_slot05_06(Object *obj) {
    if (obj->field_7e != 0) return 0;
    if (!(u8)func_801418bc(obj)) return 0;
    obj->state_0 = 1;
    obj->state_2 = 8;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 9;
    return 1;
}

int try_enter_state_7_kind_a_slot05_06(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) return 0;
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 0xa;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_b(obj);
    return 1;
}

void noop_slot00_slot05_06(Object *obj) {
}
