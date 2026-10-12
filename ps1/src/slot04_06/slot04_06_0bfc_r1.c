/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int try_enter_state_7_kind_0_slot04_06(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}

int try_start_meter_move_15a_1_direction_4_slot04_06(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->facing_side = obj->saved_facing;
    obj->field_15a = 1;
    obj->active_flag_3 = 1;
    obj->direction_index = 4;
    func_80142718(obj);
    return 1;
}

int try_enter_state_7_kind_2_slot04_06(Object *obj) {
    if (!try_action_clearing_both_flags(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 2;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_group_a(obj);
    return 1;
}
