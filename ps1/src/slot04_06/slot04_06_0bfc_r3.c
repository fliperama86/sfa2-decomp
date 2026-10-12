/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0ee4_slot04_06(Object *o) {
    if (!(u8)try_spend_meter_for_action(o)) return 0;
    o->state_0 = 1;
    o->state_1 = 0;
    o->state_3 = 0;
    o->field_157 = 0;
    o->direction_index = 0;
    o->field_6b = 0;
    o->state_2 = 7;
    o->active_flag_3 = 1;
    o->facing_side = o->saved_facing;
    o->field_15a = 8;
    o->other_lock = 0x1e;
    o->other->field_6b = 0x1a;
    set_sequence_by_side_and_mode(o, 0x39);
    return 1;
}

void start_move_8_without_check_slot04_06(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->facing_side = t;
    obj->field_15a = 8;
    obj->other_lock = 0x1e;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->direction_index = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x1a;
    set_sequence_by_side_and_mode(obj, 0x39);
}

int try_enter_state_7_kind_5_slot04_06(Object *obj) {
    if (try_action_clearing_both_flags(obj)) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return 1;
}
