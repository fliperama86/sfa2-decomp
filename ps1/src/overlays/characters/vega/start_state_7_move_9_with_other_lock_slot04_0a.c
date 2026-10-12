/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void clear_sixteen_slot_words_slot04_0a(Object *obj);

void start_state_7_move_9_with_other_lock_slot04_0a(Object *obj) {
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->other_lock = 0x28;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x24;
    set_sequence_by_side_and_mode(obj, 0x1a);
}

int try_enter_state_7_kind_0_slot04_0a(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0;
        clear_sixteen_slot_words_slot04_0a(obj);
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}

int try_enter_state_7_kind_1_slot04_0a(Object *obj) {
    int r = 0;

    if (try_action_clearing_both_flags(obj) != 0) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 1;
        clear_sixteen_slot_words_slot04_0a(obj);
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        set_direction_index_from_buttons_group_b(obj);
    }
    return r;
}
